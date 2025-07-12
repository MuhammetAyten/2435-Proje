#pragma once

class CPusItemCategory : public OdbcRecordset
{
public:
	CPusItemCategory(OdbcConnection* dbConnection, PusCategoryArray* pMap)
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("PUS_CATEGORY"); }
	virtual tstring GetColumns() { return _T("ID, Categoryname, CategoryID,Status"); }

	virtual bool Fetch()
	{
		int i = 1;
		_PUS_CATEGORY* pData = new _PUS_CATEGORY;

		_dbCommand->FetchUInt32(i++, pData->ID);
		_dbCommand->FetchString(i++, pData->CategoryName);

		_dbCommand->FetchByte(i++, pData->CategoryID);
	
		_dbCommand->FetchByte(i++, pData->Status);


		if (!m_pMap->PutData(pData->ID, pData))
			delete pData;

		return true;
	}

	PusCategoryArray* m_pMap;
};