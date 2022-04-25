# Copyright lowRISC contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

export SHELL  := /bin/bash
.DEFAULT_GOAL := all

LOCK_ROOT_DIR     ?= flock --timeout 3600 ${proj_root} --command
LOCK_SW_BUILD_DIR ?= flock --timeout 3600 ${sw_build_dir} --command

all: build run

###############################
## sim build and run targets ##
###############################
build: build_result

pre_build:
	@echo "[make]: pre_build"
	mkdir -p ${build_dir}
ifneq (${pre_build_cmds},)
	# pre_build_cmds are likely changing the in-tree sources. We hence use FLOCK
        # utility to prevent multiple builds that may be running in parallel from
        # stepping on each other. TODO: Enforce the list of pre_build_cmds is
        # identical across all build modes.
	${LOCK_ROOT_DIR} "cd ${build_dir} && ${pre_build_cmds}"
endif

gen_sv_flist: pre_build
	@echo "[make]: gen_sv_flist"
ifneq (${sv_flist_gen_cmd},)
	cd ${build_dir} && ${sv_flist_gen_cmd} ${sv_flist_gen_opts}
endif

do_build: gen_sv_flist
	@echo "[make]: build"
	cd ${sv_flist_gen_dir} && ${build_cmd} ${build_opts}

post_build: do_build
	@echo "[make]: post_build"
ifneq (${post_build_cmds},)
	cd ${build_dir} && ${post_build_cmds}
endif

build_result: post_build
	@echo "[make]: build_result"

run: run_result

pre_run:
	@echo "[make]: pre_run"
	mkdir -p ${run_dir}
ifneq (${pre_run_cmds},)
	cd ${run_dir} && ${pre_run_cmds}
endif

sw_build: pre_run
	@echo "[make]: sw_build"
ifneq (${sw_images},)
	# Initialize meson build system.
	#
	# Loop through the list of sw_images and invoke meson on each item.
	# `sw_images` is a space-separated list of tests to be built into an image.
	# Optionally, each item in the list can have additional metadata / flags using
	# the delimiter ':'. The format is as follows:
	# <path-to-sw-test>:<index>:<flag1>:<flag2>
	#
	# If no delimiter is detected, then the full string is considered to be the
	# <path-to-sw-test>. If 1 delimiter is detected, then it must be <path-to-sw-
	# test> followed by <index>. The <flag> is considered optional.
	set -e; \
	mkdir -p ${sw_build_dir}; \
	${LOCK_SW_BUILD_DIR} "cd ${proj_root} && \
		env BUILD_ROOT=${sw_build_dir} ${proj_root}/meson_init.sh"; \
	for sw_image in ${sw_images}; do \
		image=`echo $$sw_image | cut -d: -f 1`;  \
		index=`echo $$sw_image | cut -d: -f 2`; \
		flags=(`echo $$sw_image | cut -d: -f 3- --output-delimiter " "`); \
		if [[ -z $$image ]]; then \
			echo "ERROR: SW image \"$$sw_image\" is malformed."; \
			echo "Expected format: path-to-sw-test:index:optional-flags."; \
			exit 1; \
		fi; \
		if [[ $${flags[@]} =~ "prebuilt" ]]; then \
			echo "SW image \"$$image\" is prebuilt - copying sources."; \
			target_dir=`dirname ${sw_build_dir}/build-bin/$$image`; \
			mkdir -p $$target_dir; \
			cp ${proj_root}/$$image* $$target_dir/.; \
		else \
			echo "Building SW image \"$$image\"."; \
			if [[ $$index == "0" ]]; then \
				target="$$image""_${sw_build_device}"; \
				if [[ -z $${BAZEL_PYTHON_WHEELS_REPO} ]]; then \
					echo "Building \"//$$(dirname $$image)\" with bazel on network-connected machine."; \
					./bazelisk.sh build //$$(dirname $$image); \
				else \
					echo "Building \"//$$(dirname $$image)\" with bazel on air-gapped machine."; \
					bazel build \
						--distdir=$$BAZEL_DISTDIR \
						--repository_cache=$$BAZEL_CACHE \
						//$$(dirname $$image); \
				fi; \
				image_basename="$$(basename $${target}).scr.39.vmem"; \
				bazel_image_path=$$(find -L $$(bazel info output_path)/ -type f -name "$${image_basename}"); \
				target_dir="${sw_build_dir}/build-bin/$$(dirname $${target})"; \
				mkdir -p $$target_dir; \
				cp $$bazel_image_path $$target_dir/; \
			else \
				target="$$image""_export_${sw_build_device}"; \
				${LOCK_SW_BUILD_DIR} "ninja -C ${sw_build_dir}/build-out $$target"; \
			fi; \
		fi; \
	done;
endif

simulate: sw_build
	@echo "[make]: simulate"
	cd ${run_dir} && ${run_cmd} ${run_opts}

post_run: simulate
	@echo "[make]: post_run"
ifneq (${post_run_cmds},)
	cd ${run_dir} && ${post_run_cmds}
endif

run_result: post_run
	@echo "[make]: run_result"

#######################
## Load waves target ##
#######################
debug_waves:
	${debug_waves_cmd} ${debug_waves_opts}

############################
## coverage rated targets ##
############################
cov_unr_build: gen_sv_flist
	@echo "[make]: cov_unr_build"
	cd ${sv_flist_gen_dir} && ${cov_unr_build_cmd} ${cov_unr_build_opts}

cov_unr_vcs: cov_unr_build
	@echo "[make]: cov_unr"
	cd ${sv_flist_gen_dir} && ${cov_unr_run_cmd} ${cov_unr_run_opts}

cov_unr_xcelium:
	@echo "[make]: cov_unr"
	mkdir -p ${cov_unr_dir}
	cd ${cov_unr_dir} && ${cov_unr_run_cmd} ${cov_unr_run_opts}

cov_unr_merge:
	cd ${cov_unr_dir} && ${cov_merge_cmd} -init ${cov_unr_dir}/jgproject/sessionLogs/session_0/unr_imc_coverage_merge.cmd

ifeq (${SIMULATOR}, xcelium)
  cov_unr: cov_unr_xcelium cov_unr_merge
else
  cov_unr: cov_unr_vcs
endif

# Merge coverage if there are multiple builds.
cov_merge:
	@echo "[make]: cov_merge"
	${cov_merge_cmd} ${cov_merge_opts}

# Generate coverage reports.
cov_report:
	@echo "[make]: cov_report"
	${cov_report_cmd} ${cov_report_opts}

# Open coverage tool to review and create report or exclusion file.
cov_analyze:
	@echo "[make]: cov_analyze"
	${cov_analyze_cmd} ${cov_analyze_opts}

.PHONY: build \
        pre_build \
        gen_sv_flist \
        do_build \
        post_build \
        build_result \
        run \
        pre_run \
        sw_build \
        simulate \
        post_run \
        run_result \
        debug_waves \
        cov_merge \
        cov_analyze \
        cov_report
