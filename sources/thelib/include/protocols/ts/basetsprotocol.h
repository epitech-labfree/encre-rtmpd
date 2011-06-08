#ifdef HAS_PROTOCOL_TS
#ifndef _BASETSPROTOCOL_H
#define _BASETSPROTOCOL_H

#include "protocols/baseprotocol.h"
#include "protocols/ts/pidtypes.h"
#include "protocols/ts/tspacketheader.h"

#define TS_CHUNK_188 188
#define TS_CHUNK_204 204
#define TS_CHUNK_208 208


class TSProgram;
class TSBaseStream;
class TSVideoStream;
class TSAudioStream;
class TSSubtitleStream;
class BaseTSAppProtocolHandler;
class InNetTSStream;

typedef struct _PIDDescriptor {
	PIDType type;
	uint16_t pid;

	union {
		uint32_t crc;
		InNetTSStream *pStream;
	} payload;
} PIDDescriptor;

class DLLEXP BaseTsProtocol
: public BaseProtocol {
protected:
	map<uint16_t, PIDDescriptor *> _pidMapping;
	BaseTSAppProtocolHandler *_pProtocolHandler;
	bool _stepByStep;
	uint32_t _chunkSizeDetectionCount;
	uint32_t _chunkSize;
	void FreePidDescriptor(PIDDescriptor *pPIDDescriptor);
	bool DetermineChunkSize(IOBuffer &buffer);
	bool ProcessPacket(uint32_t packetHeader, IOBuffer &buffer, uint32_t maxCursor);
	bool ProcessPidTypePAT(uint32_t packetHeader,
			PIDDescriptor &pidDescriptor, uint8_t *pBuffer, uint32_t &cursor, uint32_t maxCursor);
	bool ProcessPidTypePMT(uint32_t packetHeader, PIDDescriptor &pidDescriptor,
			uint8_t *pBuffer, uint32_t &cursor, uint32_t maxCursor);

public:
	BaseTsProtocol();
	virtual ~BaseTsProtocol();
	virtual bool Initialize(Variant &parameters);
	virtual bool AllowFarProtocol(uint64_t type);
	virtual bool AllowNearProtocol(uint64_t type);

	virtual bool SignalInputData(int32_t recvAmount);
	virtual bool SignalInputData(IOBuffer &buffer);
	uint32_t GetChunkSize();
	void SetStepByStep(bool stepByStep);

	virtual void SetApplication(BaseClientApplication *pApplication);
	BaseTSAppProtocolHandler *GetProtocolHandler();
};

#endif /* !_BASETSPROTOCOL_H */
#endif
