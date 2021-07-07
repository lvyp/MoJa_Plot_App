#!/usr/bin/python
# -*- coding: utf-8 -*-
# @Time : 2021/6/11 10:12
# @Author :
# @Site : Beijing
# @File : BaiduCloudClass.py
# @Software: PyCharm
import json
import os
import requests
from aip import AipSpeech, AipNlp

global path
path = "C:/Users/98375/Desktop/tts/"


class BaiduCloud(object):
    def __init__(self):
        self.Cloud_AppID = "24343144"
        self.Cloud_APIKey = "E1GPVi0geOh8ufvROWC7NOz2"
        self.Cloud_Secret_Key = "iCvIBWCrEG2E8OBfmIiVWU3dgGZPT9B9"
        self.AipSpeechclient = AipSpeech(self.Cloud_AppID, self.Cloud_APIKey, self.Cloud_Secret_Key)

    def call_tts(self, text, audioName, humanVoice):
        try:
            paramDict = {"vol": 5, "per": humanVoice}
            result = self.AipSpeechclient.synthesis(text, "zh", 1, paramDict)
            if not isinstance(result, dict):
                global path
                if not os.path.exists(path):
                    os.mkdir(path)
                with open(path + audioName, "wb") as f:
                    f.write(result)
            return "Successful"
        except Exception as e:
            return "Exception Happened" + "Error {0}".format(str(e, encoding = 'utf-8'))

