#ifndef TEST_RPC_H
#define TEST_RPC_H

typedef int int32_t;
typedef long long int64_t;

struct _Rpc_ModelInfo{
    _Rpc_ModelInfo():Utt(nullptr),Spkid(nullptr),Language(nullptr),Gender(nullptr),Channeltype(nullptr),IvFile(nullptr),ErrMsg(nullptr){}
    ~_Rpc_ModelInfo(){
        if(Utt){
            delete[] Utt;
            Utt = nullptr;
        }
        if(Spkid){
            delete[] Spkid;
            Spkid = nullptr;
        }
        if(Language){
            delete[] Language;
            Language = nullptr;
        }
        if(Gender){
            delete[] Gender;
            Gender = nullptr;
        }
        if(Channeltype){
            delete[] Channeltype;
            Channeltype = nullptr;
        }
        if(IvFile){
            delete[] IvFile;
            IvFile = nullptr;
        }
        if(ErrMsg){
            delete[] ErrMsg;
            ErrMsg = nullptr;
        }
    }
    char *Utt;
    char *Spkid;
    int32_t Size;
    char *Language;
    char *Gender;
    char *Channeltype;
    int32_t SampRate;
    int64_t Duration;
    int64_t ValidDuration;
    char *IvFile;
    int32_t ErrCode;
    char *ErrMsg;
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
    _Rpc_SpeakerScore():Spkid(nullptr),__isset(nullptr){}
    ~_Rpc_SpeakerScore(){
        if(Spkid){
            delete[] Spkid;
            Spkid = nullptr;
        }
        if(__isset){
            delete __isset;
            __isset = nullptr;
        }
    }
    char *Spkid;
    double Score;
    _Rpc_SpeakerScore__isset* __isset;
};

struct _Rpc_TopSpeakerInfo {
    _Rpc_TopSpeakerInfo():Utt(nullptr),Scores(nullptr),ErrMsg(nullptr),__isset(nullptr){}
    ~_Rpc_TopSpeakerInfo(){
        if(Utt){
            delete[] Utt;
            Utt = nullptr;
        }
        if(Scores && Scores_size > 0){
            for(int i = 0;i < Scores_size;++i){
                delete Scores[i];
                Scores[i] = nullptr;
            }
            Scores = nullptr;
        }
        if(ErrMsg){
            delete[] ErrMsg;
            ErrMsg = nullptr;
        }
        if(__isset){
            delete __isset;
            __isset = nullptr;
        }
    }
    char *Utt;
    _Rpc_SpeakerScore** Scores;
    int Scores_size;
    int32_t Top;
    int32_t SampRate;
    int64_t Duration;
    int64_t ValidDuration;
    int32_t ErrCode;
    char *ErrMsg;
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
    _Rpc_LicenceInfo():dateStr(nullptr),fingerprint(nullptr),__isset(nullptr){}
    ~_Rpc_LicenceInfo(){
        if(dateStr){
            delete[] dateStr;
            dateStr = nullptr;
        }
        if(fingerprint){
            delete[] fingerprint;
            fingerprint = nullptr;
        }
        if(__isset){
            delete __isset;
            __isset = nullptr;
        }
    }
    char *dateStr;
    int32_t maxOccurs;
    char *fingerprint;
    int32_t RetCode;
    _Rpc_LicenceInfo__isset* __isset;
};

#endif
