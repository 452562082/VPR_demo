#ifndef TEST_RPC_H
#define TEST_RPC_H

#include <stdint.h>
#include <string>
#include <vector>

enum _Asv_Type {
	_RPC_UttInfo,
	_RPC_ModelInfo,
	_RPC_ScoreInfo,
	_RPC_SpeakerScore,
	_RPC_TopSpeakerInfo,
	_RPC_LicenceInfo
};

typedef struct __Rpc_UttInfo__isset {
  __Rpc_UttInfo__isset() : Utt(false), Duration(false), ValidDuration(false), Channels(false), SampRate(false), ErrCode(false), ErrMsg(false), RetCode(false) {}
  bool Utt :1;
  bool Duration :1;
  bool ValidDuration :1;
  bool Channels :1;
  bool SampRate :1;
  bool ErrCode :1;
  bool ErrMsg :1;
  bool RetCode :1;
} __Rpc_UttInfo__isset;

struct _Rpc_UttInfo{
	_Rpc_UttInfo() : Utt(), Duration(0), ValidDuration(0), Channels(0), SampRate(0), ErrCode(0), ErrMsg(), RetCode(0), __isset(){}
	~_Rpc_UttInfo(){}

	std::string Utt;
	int64_t Duration;
	int64_t ValidDuration;
	int32_t Channels;
	int32_t SampRate;
	int32_t ErrCode;
	std::string ErrMsg;
	int32_t RetCode;
	__Rpc_UttInfo__isset __isset;
};

struct _Rpc_ModelInfo{
	_Rpc_ModelInfo():Utt(),Spkid(),Language(),Gender(),Channeltype(),IvFile(),ErrMsg(){}
	~_Rpc_ModelInfo(){}
	std::string Utt;
	std::string Spkid;
	int32_t Size;
	std::string Language;
	std::string Gender;
	std::string Channeltype;
	int32_t SampRate;
	int64_t Duration;
	int64_t ValidDuration;
	std::string IvFile;
	int32_t ErrCode;
	std::string ErrMsg;
	int32_t RetCode;
};

typedef struct __Rpc_ScoreInfo__isset {
  __Rpc_ScoreInfo__isset() : Utt(false), Spkid(false), Score(false), SampRate(false), Duration(false), ValidDuration(false), ErrCode(false), ErrMsg(false), RetCode(false) {}
  bool Utt :1;
  bool Spkid :1;
  bool Score :1;
  bool SampRate :1;
  bool Duration :1;
  bool ValidDuration :1;
  bool ErrCode :1;
  bool ErrMsg :1;
  bool RetCode :1;
} __Rpc_ScoreInfo__isset;

struct _Rpc_ScoreInfo {
	_Rpc_ScoreInfo() :  Utt(), Spkid(), Score(0), SampRate(0), Duration(0), ValidDuration(0), ErrCode(0), ErrMsg(), RetCode(0), __isset(){}
	~_Rpc_ScoreInfo() {}

	std::string Utt;
	std::string Spkid;
	double Score;
	int32_t SampRate;
	int64_t Duration;
	int64_t ValidDuration;
	int32_t ErrCode;
	std::string ErrMsg;
	int32_t RetCode;
	__Rpc_ScoreInfo__isset __isset;
};

typedef struct __Rpc_SpeakerScore__isset {
  __Rpc_SpeakerScore__isset() : Spkid(false), Score(false) {}
  bool Spkid :1;
  bool Score :1;
} __Rpc_SpeakerScore__isset;

struct _Rpc_SpeakerScore {
    _Rpc_SpeakerScore():Spkid(),__isset(){}
    ~_Rpc_SpeakerScore(){}

    std::string Spkid;
    double Score;
    __Rpc_SpeakerScore__isset __isset;
};

typedef struct __Rpc_TopSpeakerInfo__isset {
  __Rpc_TopSpeakerInfo__isset() : Utt(false), Scores(false), Top(false), SampRate(false), Duration(false), ValidDuration(false), ErrCode(false), ErrMsg(false), RetCode(false) {}
  bool Utt :1;
  bool Scores :1;
  bool Top :1;
  bool SampRate :1;
  bool Duration :1;
  bool ValidDuration :1;
  bool ErrCode :1;
  bool ErrMsg :1;
  bool RetCode :1;
} __Rpc_TopSpeakerInfo__isset;

struct _Rpc_TopSpeakerInfo {
	_Rpc_TopSpeakerInfo():Utt(),ErrMsg(),__isset(){}
    ~_Rpc_TopSpeakerInfo(){
		Scores.clear();
	}

	std::string Utt;
	std::vector<_Rpc_SpeakerScore> Scores;
	int32_t Top;
	int32_t SampRate;
	int64_t Duration;
	int64_t ValidDuration;
	int32_t ErrCode;
	std::string ErrMsg;
	int32_t RetCode;
	__Rpc_TopSpeakerInfo__isset __isset;
};

typedef struct __Rpc_LicenceInfo__isset {
  __Rpc_LicenceInfo__isset() : dateStr(false), maxOccurs(false), fingerprint(false), RetCode(false) {}
  bool dateStr :1;
  bool maxOccurs :1;
  bool fingerprint :1;
  bool RetCode :1;
} __Rpc_LicenceInfo__isset;

struct _Rpc_LicenceInfo {
    _Rpc_LicenceInfo():dateStr(),fingerprint(),__isset(){}
    ~_Rpc_LicenceInfo(){}

    std::string dateStr;
    int32_t maxOccurs;
    std::string fingerprint;
    int32_t RetCode;
    __Rpc_LicenceInfo__isset __isset;
};

typedef struct __Rpc_ModelInfo__isset {
  __Rpc_ModelInfo__isset() : Utt(false), Spkid(false), Size(false), Language(false), Gender(false), Channeltype(false), SampRate(false), Duration(false), ValidDuration(false), IvFile(false), ErrCode(false), ErrMsg(false), RetCode(false) {}
  bool Utt :1;
  bool Spkid :1;
  bool Size :1;
  bool Language :1;
  bool Gender :1;
  bool Channeltype :1;
  bool SampRate :1;
  bool Duration :1;
  bool ValidDuration :1;
  bool IvFile :1;
  bool ErrCode :1;
  bool ErrMsg :1;
  bool RetCode :1;
} __Rpc_ModelInfo__isset;

#endif