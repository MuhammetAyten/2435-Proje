#pragma once

class CEventTimerShowSet : public OdbcRecordset
{
public:
	CEventTimerShowSet(OdbcConnection * dbConnection, EventTimerShowArray *pMap)
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("EVENT_TIMER_SHOW_LIST"); }
	virtual tstring GetColumns() 
	{
		return _T("name, status, hour, minute, days, id");
	}

	virtual bool Fetch()
	{
		int i = 1;
		_EVENT_TIMER_SHOW_LIST *pData = new _EVENT_TIMER_SHOW_LIST;
		_dbCommand->FetchString(i++, pData->name);
		_dbCommand->Fetchtbool(i++, pData->status);
		_dbCommand->FetchUInt32(i++, pData->hour);
		_dbCommand->FetchUInt32(i++, pData->minute);
		_dbCommand->FetchString(i++, pData->days);
		_dbCommand->FetchUInt32(i++, pData->id);
		if (!m_pMap->PutData(pData->id, pData)) {
			delete pData;
			return false;
		}
		return true;
	}

	EventTimerShowArray* m_pMap;
};