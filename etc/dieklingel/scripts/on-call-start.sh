#!/bin/bash
parec --raw -d alsa_output.hw_3_0.monitor --channels=2 --latency=2 --format=u8 2>/dev/null | /etc/dieklingel/scripts/pulse-aec &
