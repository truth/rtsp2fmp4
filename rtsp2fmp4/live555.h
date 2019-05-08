#pragma once

#include <time.h>
#include <map>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

#include "fmp4_muxer.h"

using websocketpp::connection_hdl;


void openURL(UsageEnvironment& env, char const* progName, char const* rtspURL);
void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString);
void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString);
void subsessionAfterPlaying(void* clientData);
void subsessionByeHandler(void* clientData);
void streamTimerHandler(void* clientData);
void setupNextSubsession(RTSPClient* rtspClient);
void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);

class FMp4Server;

class StreamClientState {
public:
	StreamClientState();
	virtual ~StreamClientState();

public:
	MediaSubsessionIterator* iter;
	MediaSession* session;
	MediaSubsession* subsession;
	TaskToken streamTimerTask;
	double duration;
};

class MyRTSPClient : public RTSPClient {
public:
	static MyRTSPClient* createNew(UsageEnvironment& env, char const* rtspURL);

protected:
	MyRTSPClient(UsageEnvironment& env, char const* rtspURL);
	virtual ~MyRTSPClient();

public:
	void connect();
	StreamClientState scs;
	std::map<connection_hdl, FMp4Muxer, std::owner_less<connection_hdl>> conns;
	static unsigned rtspClientCount;
	FMp4Server* fmp4Server;
};


