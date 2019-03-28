#
# Topology for SKL+ HDA Generic machine w/ iDISP codec only
#

# Include topology builder
include(`utils.m4')
include(`dai.m4')
include(`pipeline.m4')

# Include TLV library
include(`common/tlv.m4')

# Include Token library
include(`sof/tokens.m4')

# Include bxt DSP configuration
include(`platform/intel/bxt.m4')
include(`platform/intel/dmic.m4')

#
# Define the pipelines
#
# PCM1 ----> volume -----> iDisp1
# PCM2 ----> volume -----> iDisp2
# PCM3 ----> volume -----> iDisp3
#

dnl PIPELINE_PCM_ADD(pipeline,
dnl     pipe id, pcm, max channels, format,
dnl     frames, deadline, priority, core)

# Low Latency playback pipeline 2 on PCM 1 using max 2 channels of s32le.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-volume-playback.m4,
	2, 1, 2, s32le,
	48, 1000, 0, 0)

# Low Latency playback pipeline 3 on PCM 2 using max 2 channels of s32le.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-volume-playback.m4,
	3, 2, 2, s32le,
	48, 1000, 0, 0)

# Low Latency playback pipeline 4 on PCM 3 using max 2 channels of s32le.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-volume-playback.m4,
	4, 3, 2, s32le,
	48, 1000, 0, 0)

PIPELINE_PCM_ADD(sof/pipe-volume-capture.m4,
        5, 4, 2, s32le,
        48, 1000, 0, 0)

PIPELINE_PCM_ADD(sof/pipe-volume-capture.m4,
        6, 5, 2, s16le,
        48, 1000, 0, 0)

#
# DAIs configuration
#

dnl DAI_ADD(pipeline,
dnl     pipe id, dai type, dai_index, dai_be,
dnl     buffer, periods, format,
dnl     frames, deadline, priority, core)

# playback DAI is iDisp1 using 2 periods
# Buffers use s32le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-playback.m4,
	2, HDA, 0, iDisp1,
	PIPELINE_SOURCE_2, 2, s32le,
	48, 1000, 0, 0)

# playback DAI is iDisp2 using 2 periods
# Buffers use s32le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-playback.m4,
	3, HDA, 1, iDisp2,
	PIPELINE_SOURCE_3, 2, s32le,
	48, 1000, 0, 0)

# playback DAI is iDisp3 using 2 periods
# Buffers use s32le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-playback.m4,
	4, HDA, 2, iDisp3,
	PIPELINE_SOURCE_4, s32le,
	48, 1000, 0, 0)

# capture DAI is DMIC 0 using 2 periods
# Buffers use s32le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-capture.m4,
        6, DMIC, 0, dmic01,
        PIPELINE_SINK_5, 2, s32le,
        48, 1000, 0, 0)

# capture DAI is DMIC 1 using 2 periods
# Buffers use s16le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-capture.m4,
        7, DMIC, 1, dmic02,
        PIPELINE_SINK_6, 2, s16le,
        48, 1000, 0, 0)


# PCM Low Latency, id 0
dnl PCM_PLAYBACK_ADD(name, pcm_id, playback)
PCM_PLAYBACK_ADD(HDMI1, 1, PIPELINE_PCM_2)
PCM_PLAYBACK_ADD(HDMI2, 2, PIPELINE_PCM_3)
PCM_PLAYBACK_ADD(HDMI3, 3, PIPELINE_PCM_4)
PCM_CAPTURE_ADD(DMIC01, 6, PIPELINE_PCM_5)
PCM_CAPTURE_ADD(DMIC02, 7, PIPELINE_PCM_6)

#
# BE configurations - overrides config in ACPI if present
#

# 3 HDMI/DP outputs (ID: 1,2,3)
DAI_CONFIG(HDA, 0, 1, iDisp1)
DAI_CONFIG(HDA, 1, 2, iDisp2)
DAI_CONFIG(HDA, 2, 3, iDisp3)
DAI_CONFIG(DMIC, 0, 6, dmic01,
           DMIC_CONFIG(1, 500000, 4800000, 40, 60, 48000,
                DMIC_WORD_LENGTH(s32le), DMIC, 0,
                PDM_CONFIG(DMIC, 0, STEREO_PDM0)))
DAI_CONFIG(DMIC, 1, 7, dmic02,
           DMIC_CONFIG(1, 500000, 4800000, 40, 60, 48000,
                DMIC_WORD_LENGTH(s16le), DMIC, 1,
                PDM_CONFIG(DMIC, 1, STEREO_PDM0)))
