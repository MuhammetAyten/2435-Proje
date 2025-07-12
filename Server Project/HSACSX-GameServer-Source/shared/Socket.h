#pragma once

enum class _choffstatus { DEACTIVE, ACTIVE };

class SocketMgr;
class Socket
{
public:
	// Constructor. If fd = 0, it will be assigned 
	Socket(SOCKET fd, uint32 sendbuffersize, uint32 recvbuffersize);

	// Open a connection to another machine.
	bool Connect(const char * Address, uint32 Port);

	// Disconnect the socket.
	void Disconnect(bool m_sSocketDisconnect = false);

	// Accept from the already-set fd.
	void Accept(sockaddr_in * address);

	void ReadCallback(uint32 len);
	void WriteCallback();

	/* Implementable methods */

	// Called when data is received.
	virtual void OnRead() {}

	// Called when a connection is first successfully established.
	virtual void OnConnect() {}

	// Called when the socket is disconnected from the client (either forcibly or by the connection dropping)
	virtual void OnDisconnect() {}

	/* Send Operations */

	// Locks sending std::recursive_mutex, adds bytes, unlocks std::recursive_mutex.
	bool Send(const uint8 * Bytes, uint32 Size);

	// Burst system - Locks the sending mutex.
	INLINE  void BurstBegin() { m_writeMutex.lock(); }

	// Burst system - Adds bytes to output buffer.
	bool BurstSend(const uint8 * Bytes, uint32 Size);

	// Burst system - Pushes event to queue - do at the end of write events.
	void BurstPush();

	// Burst system - Unlocks the sending mutex.
	INLINE void BurstEnd() { m_writeMutex.unlock(); }

	// Burst system - Unlocks the sending mutex.

	/* Client Operations */

	// Get the client's ip in numerical form.
	std::string GetRemoteIP();
	INLINE sockaddr_in & GetRemoteStruct() { return m_client; }
	INLINE in_addr GetRemoteAddress() { return m_client.sin_addr; }
	INLINE uint32 GetRemotePort() { return ntohs(m_client.sin_port); }
	INLINE SOCKET GetFd() { return m_fd; }
	INLINE SocketMgr * GetSocketMgr() { return m_sClientSocket; }

	INLINE bool IsDeleted() { return m_deleted; }
	INLINE bool IsConnected() { return m_connected; }
	INLINE bool IsGetSystem() { return m_sOfflineSystem; }

	INLINE CircularBuffer& GetReadBuffer() { return readBuffer; }
	INLINE CircularBuffer& GetWriteBuffer() { return writeBuffer; }

	INLINE void SetFd(SOCKET fd) { m_fd = fd; }
	INLINE void SetSocketMgr(SocketMgr *mgr) { m_sClientSocket = mgr; }

	/* Deletion */
	void Delete();

	// Destructor.
	virtual ~Socket();

protected:
	// Called when connection is opened.
	void _OnConnect();

	SOCKET m_fd;

	CircularBuffer readBuffer, writeBuffer;

	std::recursive_mutex m_writeMutex, m_readMutex;
	// are we connected? stop from posting events.
	bool m_connected;

	// are we deleted? stop us from posting events.
	bool m_deleted;
	bool m_sOfflineSystem;

	sockaddr_in m_client;

	SocketMgr *m_sClientSocket;

public:
	const bool isOfflineStatus() const { return m_sOfflineStatus == _choffstatus::ACTIVE; }
	void SetOfflineStatus(const _choffstatus val) { m_sOfflineStatus = val; }
	
	// Set completion port that this socket will be assigned to.
	INLINE void SetCompletionPort(HANDLE cp) { m_completionPort = cp; }


	//bool Merchanting = { false };
	//bool Offline = { false };

	void SetupReadEvent();
	OverlappedStruct m_readEvent, m_writeEvent;

private:
	// Completion port socket is assigned to
	HANDLE m_completionPort;

	// Assigns the socket to his completion port.
	void AssignToCompletionPort();

public:
	/* Atomic wrapper functions for increasing read/write locks */
	INLINE void DecSendLock() 
	{ 
		Guard lock(m_writeMutex); 
		if (m_writeLock != 1)
			return;

		--m_writeLock; 
	}
	INLINE bool AcquireSendLock()
	{
		Guard lock(m_writeMutex);
		if (m_writeLock != 0)
			return false;

		++m_writeLock;
		return true;
	}
private:
	// Write lock, stops multiple write events from being posted.
	uint32 m_writeLock;
	std::recursive_mutex m_writeLockMutex;

public:
	_choffstatus m_sOfflineStatus = { _choffstatus::DEACTIVE };
};
