#!/usr/bin/env python
# -*- coding: utf-8 -*-
from BaiduCloudClass import BaiduCloud


def ttsGenerate(text, audioName, humanVoice):
    tts_cloud = BaiduCloud()
    result = tts_cloud.call_tts(text, audioName, humanVoice)
    return result
