#ifndef TEST_RPC_H
#define TEST_RPC_H

typedef int int32_t;

struct _Rpc_ModelInfo{
	char* Utt;
	char* Spkid;
	int32_t Size;
	char* Language;
	char* Gender;
	char* Channeltype;
	int32_t SampRate;
	int64_t Duration;
	int64_t ValidDuration;
	char* IvFile;
	int32_t ErrCode;
	char* ErrMsg;
	int32_t RetCode;
};

struct _Rpc_TopSpeakerInfo__isset {
  _Rpc_TopSpeakerInfo__isset() : Utt(false), Scores(false), Top(false), SampRate(false), Duration(false), ValidDuration(false), ErrCode(false), ErrMsg(false), RetCode(false) {}
  bool Utt :1;
  bool Scores :1;
  bool Top :1;
  bool SampRate :1;
  bool Duration :1;
  bool ValidDuration :1;
  bool ErrCode :1;
  bool ErrMsg :1;
  bool RetCode :1;
};

typedef struct _Rpc_SpeakerScore__isset {
  _Rpc_SpeakerScore__isset() : Spkid(false), Score(false) {}
  bool Spkid :1;
  bool Score :1;
} _Rpc_SpeakerScore__isset;

struct _Rpc_SpeakerScore {
    char* Spkid;
    double Score;
    _Rpc_SpeakerScore__isset* __isset;
};

struct _Rpc_TopSpeakerInfo {
	char* Utt;
	_Rpc_SpeakerScore* Scores;
	int Scores_size;
	int32_t Top;
	int32_t SampRate;
	int64_t Duration;
	int64_t ValidDuration;
	int32_t ErrCode;
	char* ErrMsg;
	int32_t RetCode;
	_Rpc_TopSpeakerInfo__isset* __isset;
};

typedef struct _Rpc_LicenceInfo__isset {
  _Rpc_LicenceInfo__isset() : dateStr(false), maxOccurs(false), fingerprint(false), RetCode(false) {}
  bool dateStr :1;
  bool maxOccurs :1;
  bool fingerprint :1;
  bool RetCode :1;
} _Rpc_LicenceInfo__isset;

struct _Rpc_LicenceInfo {
    const char* dateStr;
    int32_t maxOccurs;
    const char* fingerprint;
    int32_t RetCode;
    _Rpc_LicenceInfo__isset* __isset;
};

#endif
