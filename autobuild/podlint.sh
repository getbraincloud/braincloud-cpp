#!/bin/bash
export LANG=en_US.UTF-8
pod cache clean --all
pod lib lint --use-libraries --allow-warnings --verbose
