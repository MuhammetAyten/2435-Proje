#pragma once

class CPusItemSet : public OdbcRecordset
{
public:
	CPusItemSet(OdbcConnection* dbConnection, PusItemArray* pMap)
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("PUS_ITEMS"); }
	virtual tstring GetColumns() { return _T("ID, ItemID, Price, BuyCount, Category,PriceType"); }

	virtual bool Fetch()
	{
		int i = 1;
		_PUS_ITEM* pData = new _PUS_ITEM;

		_dbCommand->FetchUInt32(i++, pData->ID);
		_dbCommand->FetchUInt32(i++, pData->ItemID);
		_dbCommand->FetchUInt32(i++, pData->Price);
		_dbCommand->FetchUInt32(i++, pData->BuyCount);
		_dbCommand->FetchByte(i++, pData->Cat);
		_dbCommand->FetchByte(i++, pData->PriceType);

		if (!m_pMap->PutData(pData->ID, pData))
			delete pData;

		return true;
	}

	PusItemArray* m_pMap;
};