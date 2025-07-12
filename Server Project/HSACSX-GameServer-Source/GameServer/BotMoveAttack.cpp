#include "StdAfx.h"

//void CBot::RegionFindAttackProcess()
//{
//	if ((m_sMoveRegionAttackTime - UNIXTIME2) < 1 * SECOND)
//		return;
//
//	std::vector<Unit*> casted_member;
//	std::vector<uint16> unitList;
//	g_pMain->GetUnitListFromSurroundingRegions(this, &unitList);
//
//	foreach(itr, unitList)
//	{
//		Unit* pTarget = g_pMain->GetUnitPtr(*itr, GetZoneID());
//
//		if (pTarget == nullptr)
//			continue;
//
//		if (this != pTarget
//			&& !pTarget->isDead()
//			&& !pTarget->isBlinking()
//			&& pTarget->isAttackable())
//			casted_member.push_back(pTarget);
//	}
//
//	int iValue = 0;
//	float UnitX = 0, UnitY = 0, UnitZ = 0;
//
//	int16 sTargetID = -1;
//	__Vector3 vBot, vUnit, vDistance, vRealDistance;
//	float fSearchRange = 45.0f;
//	float Mesafe = 0.0f, EnYakinMesafe = 0.0f;
//	foreach(itr, casted_member)
//	{
//		Unit* pTarget = *itr; // it's checked above, not much need to check it again
//
//		if (pTarget == nullptr)
//			continue;
//
//		if (pTarget->isDead()
//			|| pTarget->isPlayer() && TO_USER(pTarget)->GetNation() == GetNation()
//			|| pTarget->isPlayer() && !TO_USER(pTarget)->isInPKZone()
//			|| pTarget->isPlayer() && TO_USER(pTarget)->isInGenie()
//			|| pTarget->isPlayer() && TO_USER(pTarget)->isGM()
//			|| pTarget->GetZoneID() != GetZoneID()
//			|| pTarget->isNPC()
//			|| pTarget->isBot() && TO_BOT(pTarget)->GetNation() == GetNation()
//			|| pTarget->isBot() && !TO_BOT(pTarget)->isInPKZone())
//			continue;
//
//		Mesafe = pow(pTarget->GetX() + ((myrand(0, 2000) - 1000.0f) / 500.0f) - GetX(), 2.0f) + pow(pTarget->GetZ() + ((myrand(0, 2000) - 1000.0f) / 500.0f) - GetZ(), 2.0f);
//
//		if (EnYakinMesafe != 0.0f && Mesafe > EnYakinMesafe)
//			continue;
//
//		sTargetID = pTarget->GetID();
//		UnitX = pTarget->GetX();
//		UnitZ = pTarget->GetZ();
//		UnitY = pTarget->GetY();
//		EnYakinMesafe = Mesafe;
//		/*float fDis = GetDistanceSqrt(pTarget);
//		if (fDis > fSearchRange)
//			continue;
//
//		if (sTargetID > -1)
//			continue;
//
//		sTargetID = pTarget->GetID();
//		UnitX = pTarget->GetX();
//		UnitZ = pTarget->GetZ();
//		UnitY = pTarget->GetY();*/
//	}
//
//	if (sTargetID == int16(-1)
//		|| UnitX == 0
//		|| UnitY == 0
//		|| UnitZ == 0)
//	{
//		m_sMoveRegionAttackTime = ULONGLONG(UNIXTIME2 + (5 * SECOND));
//		return;
//	}
//
//	vBot.Set(GetX(), UnitY, GetZ());
//	vUnit.Set(UnitX + ((myrand(0, 2000) - 1000.0f) / 500.0f), UnitY, UnitZ + ((myrand(0, 2000) - 1000.0f) / 500.0f));
//
//	if (m_sTargetID != sTargetID)
//	{
//		m_TargetChanged = true;
//		m_sTargetID = sTargetID;
//		/*m_oldx = vUnit.x + ((myrand(0, 2000) - 1000.0f) / 500.0f);
//		m_oldz = vUnit.z + ((myrand(0, 2000) - 1000.0f) / 500.0f);
//		m_oldy = vUnit.y;*/
//	}
//
//	vDistance = vUnit - vBot;
//	vRealDistance = vDistance;
//	vDistance.Normalize();
//
//	float sSpeed = m_sSpeed;
//	uint8 sRunTime = 1;
//	bool sRunFinish = false;
//	vDistance *= sSpeed / 10.0f;
//
//	if (echo == uint8(0)
//		&& vDistance.Magnitude() < vRealDistance.Magnitude()
//		&& (vDistance * EnYakinMesafe).Magnitude() < vRealDistance.Magnitude())
//	{
//		vDistance *= EnYakinMesafe;
//		sRunTime = (uint8)EnYakinMesafe;
//	}
//	else if (vDistance.Magnitude() > vRealDistance.Magnitude()
//		|| vDistance.Magnitude() == vRealDistance.Magnitude())
//	{
//		sRunFinish = true;
//		vDistance = vRealDistance;
//	}
//
//	if (m_TargetChanged)
//	{
//		m_TargetChanged = false;
//		echo = uint8(1);
//		m_sMoveRegionAttackTime = ULONGLONG(UNIXTIME2 + (sRunTime * SECOND));
//	}
//	else if (sRunFinish)
//	{
//		echo = uint8(0);
//		m_sMoveRegionAttackTime = ULONGLONG(UNIXTIME2 + (myrand(1, sRunTime) * SECOND));
//	}
//	else
//	{
//		echo = uint8(3);
//		m_sMoveRegionAttackTime = ULONGLONG(UNIXTIME2 + (sRunTime * SECOND));
//	}
//
//	uint16 will_x, will_z, will_y;
//	will_x = uint16((vBot + vDistance).x * 10.0f);
//	will_y = uint16(vUnit.y * 10.0f);
//	will_z = uint16((vBot + vDistance).z * 10.0f);
//	m_sMoveRegionAttackTime = ULONGLONG(UNIXTIME2 + (2 * SECOND));
//
//	if (isRogue())
//		RegionGetAssasinDamageMagic(sTargetID, (vBot + vDistance).x, vUnit.y, (vBot + vDistance).z, will_x, will_y, will_z, sSpeed, echo);
//	else if (isWarrior())
//		RegionGetWarriorDamageMagic(sTargetID, (vBot + vDistance).x, vUnit.y, (vBot + vDistance).z, will_x, will_y, will_z, sSpeed, echo);
//	else if (isMage())
//	{
//		int nRandom = myrand(1, 3);
//		switch (nRandom)
//		{
//		case 1:
//			RegionGetFlameMageDamageMagic(sTargetID, (vBot + vDistance).x, vUnit.y, (vBot + vDistance).z, will_x, will_y, will_z, sSpeed, echo);
//			break;
//		case 2:
//			RegionGetLightningMageDamageMagic(sTargetID, (vBot + vDistance).x, vUnit.y, (vBot + vDistance).z, will_x, will_y, will_z, sSpeed, echo);
//			break;
//		default:
//			RegionGetGlacierMageDamageMagic(sTargetID, (vBot + vDistance).x, vUnit.y, (vBot + vDistance).z, will_x, will_y, will_z, sSpeed, echo);
//			break;
//		}
//	}
//	else if (isPriest())
//		RegionGetPriestDamageMagic(sTargetID, (vBot + vDistance).x, vUnit.y, (vBot + vDistance).z, will_x, will_y, will_z, sSpeed, echo);
//}

void CBot::RegionFindAttackProcess()
{
	if ((m_sMoveRegionAttackTime - UNIXTIME2) < 1 * SECOND)
		return;

	std::vector<Unit*> attackableUnits;
	std::vector<uint16> unitList;
	g_pMain->GetUnitListFromSurroundingRegions(this, &unitList);

	for (uint16 unitID : unitList)
	{
		Unit* pTarget = g_pMain->GetUnitPtr(unitID, GetZoneID());

		if (IsValidTarget(pTarget))
			attackableUnits.push_back(pTarget);
	}

	Unit* nearestTarget = GetNearestTarget(attackableUnits);
	if (!nearestTarget)
	{
		m_sMoveRegionAttackTime = ULONGLONG(UNIXTIME2 + (5 * SECOND));
		return;
	}

	__Vector3 botPosition(GetX(), GetY(), GetZ());
	__Vector3 targetPosition(nearestTarget->GetX(), nearestTarget->GetY(), nearestTarget->GetZ());

	if (m_sTargetID != nearestTarget->GetID())
	{
		m_TargetChanged = true;
		m_sTargetID = nearestTarget->GetID();
	}

	float distanceToTarget = (targetPosition - botPosition).Magnitude();
	float searchRange = 45.0f;

	if (distanceToTarget > searchRange)
	{
		m_sMoveRegionAttackTime = ULONGLONG(UNIXTIME2 + (5 * SECOND));
		return;
	}

	// Calculate new position
	__Vector3 newPosition = CalculateNewPosition(botPosition, targetPosition);
	uint16 will_x = static_cast<uint16>(newPosition.x * 10.0f);
	uint16 will_y = static_cast<uint16>(newPosition.y * 10.0f);
	uint16 will_z = static_cast<uint16>(newPosition.z * 10.0f);

	HandleAttack(nearestTarget, newPosition);
}

bool CBot::IsValidTarget(Unit* pTarget)
{
	if (!pTarget)
		return false;

	if (pTarget->isDead()
		|| (pTarget->isPlayer() && TO_USER(pTarget)->GetNation() == GetNation())
		|| (pTarget->isPlayer() && !TO_USER(pTarget)->isInPKZone())
		|| (pTarget->isPlayer() && TO_USER(pTarget)->isInGenie())
		|| (pTarget->isPlayer() && TO_USER(pTarget)->isGM())
		|| pTarget->GetZoneID() != GetZoneID()
		|| pTarget->isNPC()
		|| (pTarget->isBot() && TO_BOT(pTarget)->GetNation() == GetNation())
		|| (pTarget->isBot() && !TO_BOT(pTarget)->isInPKZone()))
		return false;
	return true;
}

Unit* CBot::GetNearestTarget(const std::vector<Unit*>& units)
{
	Unit* nearest = nullptr;
	float closestDistance = std::numeric_limits<float>::max();

	for (Unit* unit : units)
	{
		float distance = GetDistanceToUnit(unit);
		if (distance < closestDistance)
		{
			closestDistance = distance;
			nearest = unit;
		}
	}

	return nearest;
}

float CBot::GetDistanceToUnit(Unit* unit)
{
	__Vector3 botPosition(GetX(), GetY(), GetZ());
	__Vector3 targetPosition(unit->GetX(), unit->GetY(), unit->GetZ());

	return (targetPosition - botPosition).Magnitude();
}

__Vector3 CBot::CalculateNewPosition(const __Vector3& botPosition, const __Vector3& targetPosition)
{
	__Vector3 newPosition = targetPosition;
	newPosition.x += (myrand(0, 2000) - 1000.0f) / 500.0f;
	newPosition.z += (myrand(0, 2000) - 1000.0f) / 500.0f;

	return newPosition;
}

void CBot::HandleAttack(Unit* pTarget, const __Vector3& newPosition)
{
	float sSpeed = m_sSpeed / 10.0f;
	uint8 sRunTime = 1;
	bool sRunFinish = false;

	__Vector3 botPosition(GetX(), GetY(), GetZ());
	__Vector3 vDistance = newPosition - botPosition;
	__Vector3 vRealDistance = vDistance;
	vDistance.Normalize();

	if (vDistance.Magnitude() < vRealDistance.Magnitude())
	{
		vDistance *= vRealDistance.Magnitude();
		sRunTime = static_cast<uint8>(vRealDistance.Magnitude());
	}
	else
	{
		sRunFinish = true;
		vDistance = vRealDistance;
	}

	if (m_TargetChanged)
	{
		m_TargetChanged = false;
		m_sMoveRegionAttackTime = ULONGLONG(UNIXTIME2 + (sRunTime * SECOND));
	}
	else if (sRunFinish)
	{
		m_sMoveRegionAttackTime = ULONGLONG(UNIXTIME2 + (myrand(1, sRunTime) * SECOND));
	}
	else
	{
		m_sMoveRegionAttackTime = ULONGLONG(UNIXTIME2 + (sRunTime * SECOND));
	}

	uint16 will_x = static_cast<uint16>(newPosition.x * 10.0f);
	uint16 will_y = static_cast<uint16>(newPosition.y * 10.0f);
	uint16 will_z = static_cast<uint16>(newPosition.z * 10.0f);

	if (isRogue())
		RegionGetAssasinDamageMagic(pTarget->GetID(), newPosition.x, newPosition.y, newPosition.z, will_x, will_y, will_z, sSpeed, 0);
	else if (isWarrior())
		RegionGetWarriorDamageMagic(pTarget->GetID(), newPosition.x, newPosition.y, newPosition.z, will_x, will_y, will_z, sSpeed, 0);
	else if (isMage())
	{
		int nRandom = myrand(1, 3);
		switch (nRandom)
		{
		case 1:
			RegionGetFlameMageDamageMagic(pTarget->GetID(), newPosition.x, newPosition.y, newPosition.z, will_x, will_y, will_z, sSpeed, 0);
			break;
		case 2:
			RegionGetLightningMageDamageMagic(pTarget->GetID(), newPosition.x, newPosition.y, newPosition.z, will_x, will_y, will_z, sSpeed, 0);
			break;
		default:
			RegionGetGlacierMageDamageMagic(pTarget->GetID(), newPosition.x, newPosition.y, newPosition.z, will_x, will_y, will_z, sSpeed, 0);
			break;
		}
	}
	else if (isPriest())
		RegionGetPriestDamageMagic(pTarget->GetID(), newPosition.x, newPosition.y, newPosition.z, will_x, will_y, will_z, sSpeed, 0);
}

void CBot::RegionGetAssasinDamageMagic(int16 tid, float X, float Y, float Z, int16 will_x, int16 will_y, int16 will_z, float sSpeed, int8 echo)
{
	if (tid < 0)
		return;

	int32 sSkillID = 0, bSkillID = 0;
	uint8 weaponSlots[] = { LEFTHAND, RIGHTHAND };
	bool ArrowMagics = false, DaggersMagics = false;

	_ITEM_TABLE pTable = GetItemPrototype(RIGHTHAND);
	if (pTable.isnull())
	{
		pTable = GetItemPrototype(LEFTHAND);
		if (!pTable.isnull())
		{
			if (pTable.isDagger())
				RegionGetAssasinDaggerDamageMagic(tid, X, Y, Z, will_x, will_y, will_z, sSpeed, echo);
		}
	}
	else if (!pTable.isnull())
	{
		if (pTable.isBow() || pTable.isCrossBow())
			RegionGetAssasinArrowDamageMagic(tid, X, Y, Z, will_x, will_y, will_z, sSpeed, echo);
		else if (pTable.isShield())
		{
			pTable = GetItemPrototype(LEFTHAND);
			if (!pTable.isnull())
			{
				if (pTable.isDagger())
					RegionGetAssasinDaggerDamageMagic(tid, X, Y, Z, will_x, will_y, will_z, sSpeed, echo);
			}
		}
		else if (pTable.isDagger())
			RegionGetAssasinDaggerDamageMagic(tid, X, Y, Z, will_x, will_y, will_z, sSpeed, echo);
	}
}

void CBot::RegionGetAssasinArrowDamageMagic(int16 tid, float X, float Y, float Z, int16 will_x, int16 will_y, int16 will_z, float sSpeed, int8 echo)
{
	if (tid < 0)
		return;

	uint32 sSkillID = 0;
	switch (GetLevel())
	{
	case 1:
	case 2:
	case 3:
		sSkillID = 107003;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		sSkillID = 107003;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
		sSkillID = 107500;
		break;
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
		sSkillID = 107525;
		break;
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		sSkillID = 107540;
		break;
	case 52:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			sSkillID = 107540;
			break;
		case 1:
			sSkillID = 107552;
			break;
		}
	}break;
	case 53:
	case 54:
	case 55:
	case 56:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			sSkillID = 107540;
			break;
		case 1:
			sSkillID = 107552;
			break;
		}
	}break;
	case 57:
	case 58:
	case 59:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			sSkillID = 107557;
			break;
		case 1:
			sSkillID = 107552;
			break;
		}
	}break;
	case 60:
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			sSkillID = 108552;
			break;
		case 1:
			sSkillID = 108560;
			break;
		}
	}break;
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			sSkillID = 108552;
			break;
		case 1:
			sSkillID = 108570;
			break;
		}
	}break;
	case 80:
	case 81:
	case 82:
	case 83:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			sSkillID = 108552;
			break;
		case 1:
			sSkillID = 108570;
			break;
		case 2:
			sSkillID = 108580;
			break;
		case 3:
			sSkillID = 108585;
			break;
		}
	}break;
	}

	if (GetNation() == ELMORAD)
		sSkillID += 100000;

	if (GetLevel() <= MAX_LEVEL_ARDREAM)
	{
		if (GetNation() == KARUS)
		{
			if (sSkillID > 108000)
				sSkillID -= 1000;
		}
		else
		{
			if (sSkillID > 208000)
				sSkillID -= 1000;
		}
	}

	auto pSkill = g_pMain->GetMagicPtr(sSkillID);
	if (pSkill.isnull())
		return;

	Unit* pUnit = g_pMain->GetUnitPtr(tid);
	if (pUnit == nullptr
		|| pUnit->isDead()
		|| pUnit->isPlayer() && TO_USER(pUnit)->isGM())
		return;

	float sRange = (float)pSkill.sRange > 0 ? pSkill.sRange : 7.0f;
	float fDis = GetDistanceSqrt(pUnit);
	if (fDis > sRange)
		return;

	if ((uint32)UNIXTIME >= (m_sSkillCoolDown[0]))
	{
		MagicPacket(MAGIC_CASTING, sSkillID, GetID(), pUnit->GetID(), (uint16)GetX(), (uint16)GetY(), (uint16)GetZ());
		MagicPacket(MAGIC_EFFECTING, sSkillID, GetID(), pUnit->GetID(), (uint16)GetX(), (uint16)GetY(), (uint16)GetZ());
		m_sSkillCoolDown[0] = (uint32)UNIXTIME + 3; m_sSpeed = NULL;
	}
}

void CBot::RegionGetAssasinDaggerDamageMagic(int16 tid, float X, float Y, float Z, int16 will_x, int16 will_y, int16 will_z, float sSpeed, int8 echo)
{
	if (tid < 0)
		return;

	uint32 bSkillID = 0;
	switch (GetLevel())
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		bSkillID = 101001;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108600;
			break;
		case 1:
			bSkillID = 108005;
			break;
		default:
			bSkillID = 108615;
			break;
		}
	}break;
	case 15:
	{
		int nRandom = myrand(0, 2);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108600;
			break;
		case 1:
			bSkillID = 108005;
			break;
		default:
			bSkillID = 108615;
			break;
		}
	}break;
	case 16:
	case 17:
	case 18:
	case 19:
	{
		int nRandom = myrand(0, 2);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108600;
			break;
		case 1:
			bSkillID = 108005;
			break;
		default:
			bSkillID = 108615;
			break;
		}
	}break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108600;
			break;
		case 1:
			bSkillID = 108005;
			break;
		case 2:
			bSkillID = 108615;
			break;
		default:
			bSkillID = 108620;
			break;
		}
	}break;
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108005;
			break;
		case 1:
			bSkillID = 108600;
			break;
		case 2:
			bSkillID = 108615;
			break;
		default:
			bSkillID = 108620;
			break;
		}
	}break;
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	{
		int nRandom = myrand(0, 4);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108005;
			break;
		case 1:
			bSkillID = 108600;
			break;
		case 2:
			bSkillID = 108615;
			break;
		case 3:
			bSkillID = 108620;
			break;
		default:
			bSkillID = 108635;
			break;
		}
	}break;
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	{
		int nRandom = myrand(0, 5);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108005;
			break;
		case 1:
			bSkillID = 108600;
			break;
		case 2:
			bSkillID = 108615;
			break;
		case 3:
			bSkillID = 108620;
			break;
		case 4:
			bSkillID = 108635;
			break;
		default:
			bSkillID = 108640;
			break;
		}
	}break;
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	{
		int nRandom = myrand(0, 5);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108005;
			break;
		case 1:
			bSkillID = 108600;
			break;
		case 2:
			bSkillID = 108615;
			break;
		case 3:
			bSkillID = 108620;
			break;
		case 4:
			bSkillID = 108635;
			break;
		default:
			bSkillID = 108640;
			break;
		}
	}break;
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	{
		int nRandom = myrand(0, 6);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108005;
			break;
		case 1:
			bSkillID = 108600;
			break;
		case 2:
			bSkillID = 108615;
			break;
		case 3:
			bSkillID = 108620;
			break;
		case 4:
			bSkillID = 108635;
			break;
		case 5:
			bSkillID = 108640;
			break;
		default:
			bSkillID = 108655;
			break;
		}
	}break;
	case 70:
	{
		int nRandom = myrand(0, 8);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108005;
			break;
		case 1:
			bSkillID = 108600;
			break;
		case 2:
			bSkillID = 108615;
			break;
		case 3:
			bSkillID = 108620;
			break;
		case 4:
			bSkillID = 108635;
			break;
		case 5:
			bSkillID = 108640;
			break;
		case 6:
			bSkillID = 108655;
			break;
		case 7:
			bSkillID = 108656;
			break;
		default:
			bSkillID = 108670;
			break;
		}
	}break;
	case 71:
	case 72:
	case 73:
	case 74:
	{
		int nRandom = myrand(0, 8);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108005;
			break;
		case 1:
			bSkillID = 108600;
			break;
		case 2:
			bSkillID = 108615;
			break;
		case 3:
			bSkillID = 108620;
			break;
		case 4:
			bSkillID = 108635;
			break;
		case 5:
			bSkillID = 108640;
			break;
		case 6:
			bSkillID = 108655;
			break;
		case 7:
			bSkillID = 108656;
			break;
		default:
			bSkillID = 108670;
			break;
		}
	}break;
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	{
		int nRandom = myrand(0, 9);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108005;
			break;
		case 1:
			bSkillID = 108600;
			break;
		case 2:
			bSkillID = 108615;
			break;
		case 3:
			bSkillID = 108620;
			break;
		case 4:
			bSkillID = 108635;
			break;
		case 5:
			bSkillID = 108640;
			break;
		case 6:
			bSkillID = 108655;
			break;
		case 7:
			bSkillID = 108656;
			break;
		case 8:
			bSkillID = 108670;
			break;
		default:
			bSkillID = 108675;
			break;
		}
	}break;
	case 80:
	case 81:
	case 82:
	case 83:
	{
		int nRandom = myrand(0, 11);
		switch (nRandom)
		{
		case 0:
			bSkillID = 108005;
			break;
		case 1:
			bSkillID = 108600;
			break;
		case 2:
			bSkillID = 108615;
			break;
		case 3:
			bSkillID = 108620;
			break;
		case 4:
			bSkillID = 108635;
			break;
		case 5:
			bSkillID = 108640;
			break;
		case 6:
			bSkillID = 108655;
			break;
		case 7:
			bSkillID = 108656;
			break;
		case 8:
			bSkillID = 108670;
			break;
		case 9:
			bSkillID = 108675;
			break;
		case 10:
			bSkillID = 108680;
			break;
		default:
			bSkillID = 108685;
			break;
		}
	}break;
	}

	if (GetNation() == ELMORAD)
		bSkillID += 100000;

	if (GetLevel() <= MAX_LEVEL_ARDREAM)
	{
		if (GetNation() == KARUS)
		{
			if (bSkillID > 108000)
				bSkillID -= 1000;
		}
		else
		{
			if (bSkillID > 208000)
				bSkillID -= 1000;
		}
	}

	auto pSkill = g_pMain->GetMagicPtr(bSkillID);
	if (pSkill.isnull())
		return;

	Unit* pUnit = g_pMain->GetUnitPtr(tid);
	if (pUnit == nullptr
		|| pUnit->isDead()
		|| pUnit->isPlayer() && TO_USER(pUnit)->isGM())
		return;

	float sRange = (float)pSkill.sRange > 0 ? pSkill.sRange : 7.0f;
	float fDis = GetDistanceSqrt(pUnit);
	if (fDis > sRange)
		return;

	if ((uint32)UNIXTIME >= (m_sSkillCoolDown[0]))
	{
		MagicPacket(MAGIC_CASTING, bSkillID, GetID(), pUnit->GetID(), (uint16)GetX(), (uint16)GetY(), (uint16)GetZ());
		MagicPacket(MAGIC_EFFECTING, bSkillID, GetID(), pUnit->GetID(), (uint16)GetX(), (uint16)GetY(), (uint16)GetZ());
		m_sSkillCoolDown[0] = (uint32)UNIXTIME + 2;
	}
}

void CBot::RegionGetWarriorDamageMagic(int16 tid, float X, float Y, float Z, int16 will_x, int16 will_y, int16 will_z, float sSpeed, int8 echo)
{
	if (tid < 0)
		return;

	uint32 bSkillID = 0;
	switch (GetLevel())
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		bSkillID = 101001;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
		bSkillID = 105505;
		break;
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
		bSkillID = 105525;
		break;
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
		bSkillID = 105545;
		break;
	case 55:
	case 56:
		bSkillID = 105555;
		break;
	case 57:
	case 58:
	case 59:
		bSkillID = 105557;
		break;
	case 60:
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
		bSkillID = 106560;
		break;
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
		bSkillID = 106570;
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			bSkillID = 106570;
			break;
		case 1:
			bSkillID = 106575;
			break;
		}
	}break;
	case 80:
	case 81:
		bSkillID = 106580;
		break;
	case 82:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			bSkillID = 106580;
			break;
		case 1:
			bSkillID = 106782;
			break;
		}
	}break;
	case 83:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			bSkillID = 106580;
			break;
		case 1:
			bSkillID = 106782;
			break;
		}
	}break;
	}

	if (GetNation() == ELMORAD)
		bSkillID += 100000;

	if (GetLevel() <= MAX_LEVEL_ARDREAM)
	{
		if (GetNation() == KARUS)
		{
			if (bSkillID > 106000)
				bSkillID -= 1000;
		}
		else
		{
			if (bSkillID > 206000)
				bSkillID -= 1000;
		}
	}

	_MAGIC_TABLE* pSkill = g_pMain->m_MagicTableArray.GetData(bSkillID);
	if (pSkill == nullptr)
		return;

	Unit* pUnit = g_pMain->GetUnitPtr(tid);
	if (pUnit == nullptr
		|| pUnit->isDead()
		|| pUnit->isPlayer() && TO_USER(pUnit)->isGM())
		return;

	float sRange = (float)pSkill->sRange > 0 ? pSkill->sRange : 7.0f;
	float fDis = GetDistanceSqrt(pUnit);
	if (fDis > sRange)
		return;

	if ((uint32)UNIXTIME >= (m_sSkillCoolDown[0]))
	{
		MagicPacket(MAGIC_CASTING, bSkillID, GetID(), pUnit->GetID(), (uint16)GetX(), (uint16)GetY(), (uint16)GetZ());
		MagicPacket(MAGIC_EFFECTING, bSkillID, GetID(), pUnit->GetID(), (uint16)GetX(), (uint16)GetY(), (uint16)GetZ());
		m_sSkillCoolDown[0] = (uint32)UNIXTIME + 2;
	}
}

void CBot::RegionGetFlameMageDamageMagic(int16 tid, float X, float Y, float Z, int16 will_x, int16 will_y, int16 will_z, float sSpeed, int8 echo)
{
	if (tid < 0)
		return;

	uint32 sSkillID = 0;
	switch (GetLevel())
	{
	case 1:
	case 2:
	case 3:
	case 4:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		}
	}break;
	case 5:
	case 6:
	{
		int nRandom = myrand(0, 2);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		case 2:
			sSkillID = 109005;
			break;
		}
	}break;
	case 7:
	case 8:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		case 2:
			sSkillID = 109005;
			break;
		case 3:
			sSkillID = 109007;
			break;
		}
	}break;
	case 9:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		case 2:
			sSkillID = 109005;
			break;
		case 3:
			sSkillID = 109007;
			break;
		case 4:
			sSkillID = 109009;
			break;
		case 5:
			sSkillID = 109010;
			break;
		}
	}break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		}
	}break;
	case 15:
	case 16:
	case 17:
	{
		int nRandom = myrand(0, 2);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		}
	}break;
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		}
	}break;
	case 27:
	{
		int nRandom = myrand(0, 4);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		}
	}break;
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	{
		int nRandom = myrand(0, 4);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		}
	}break;
	case 33:
	{
		int nRandom = myrand(0, 5);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		}
	}break;
	case 34:
	case 35:
	{
		int nRandom = myrand(0, 6);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		}
	}break;
	case 36:
	case 37:
	case 38:
	{
		int nRandom = myrand(0, 6);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		}
	}break;
	case 39:
	{
		int nRandom = myrand(0, 7);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		}
	}break;
	case 40:
	case 41:
	{
		int nRandom = myrand(0, 7);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		}
	}break;
	case 42:
	{
		int nRandom = myrand(0, 8);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		}
	}break;
	case 43:
	{
		int nRandom = myrand(0, 9);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		}
	}break;
	case 44:
	case 45:
	{
		int nRandom = myrand(0, 10);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		}
	}break;
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	{
		int nRandom = myrand(0, 10);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		}
	}break;
	case 51:
	{
		int nRandom = myrand(0, 11);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		}
	}break;
	case 52:
	case 53:
	case 54:
	case 55:
	{
		int nRandom = myrand(0, 11);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		}
	}break;
	case 56:
	{
		int nRandom = myrand(0, 12);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		case 12:
			sSkillID = 109556;
			break;
		}
	}break;
	case 57:
	{
		int nRandom = myrand(0, 13);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		case 12:
			sSkillID = 109556;
			break;
		case 13:
			sSkillID = 109557;
			break;
		}
	}break;
	case 58:
	case 59:
	{
		int nRandom = myrand(0, 13);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		case 12:
			sSkillID = 109556;
			break;
		case 13:
			sSkillID = 109557;
			break;
		}
	}break;
	case 60:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110551;
			break;
		case 12:
			sSkillID = 110556;
			break;
		case 13:
			sSkillID = 110557;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110551;
			break;
		case 12:
			sSkillID = 110556;
			break;
		case 13:
			sSkillID = 110557;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 70:
	case 71:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110551;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 72:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110572;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110572;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 80:
	case 81:
	case 82:
	case 83:
	{
		int nRandom = myrand(8, 15);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110572;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		case 15:
			sSkillID = 110575;
			break;
		}
	}break;
	}

	if (GetNation() == ELMORAD)
		sSkillID += 100000;

	if (GetLevel() <= MAX_LEVEL_ARDREAM)
	{
		if (GetNation() == KARUS)
		{
			if (sSkillID > 110000)
				sSkillID -= 1000;
		}
		else
		{
			if (sSkillID > 210000)
				sSkillID -= 1000;
		}
	}

	_MAGIC_TABLE* pSkill = g_pMain->m_MagicTableArray.GetData(sSkillID);
	if (pSkill == nullptr)
		return;

	Unit* pUnit = g_pMain->GetUnitPtr(tid);
	if (pUnit == nullptr
		|| pUnit->isDead()
		|| pUnit->isPlayer() && TO_USER(pUnit)->isGM())
		return;

	float sRange = (float)pSkill->sRange > 0 ? pSkill->sRange : 7.0f;
	float fDis = GetDistanceSqrt(pUnit);
	if (fDis > sRange)
		return;

	if ((uint32)UNIXTIME >= (m_sSkillCoolDown[0]))
	{
		MagicPacket(MAGIC_CASTING, sSkillID, GetID(), pUnit->GetID(), (uint16)pUnit->GetX(), (uint16)pUnit->GetY(), (uint16)pUnit->GetZ());
		MagicPacket(MAGIC_EFFECTING, sSkillID, GetID(), pUnit->GetID(), (uint16)pUnit->GetX(), (uint16)pUnit->GetY(), (uint16)pUnit->GetZ());
		m_sSkillCoolDown[0] = (uint32)UNIXTIME + 3; m_sSpeed = NULL;
	}
}

void CBot::RegionGetGlacierMageDamageMagic(int16 tid, float X, float Y, float Z, int16 will_x, int16 will_y, int16 will_z, float sSpeed, int8 echo)
{
	if (tid < 0)
		return;

	uint32 sSkillID = 0;
	switch (GetLevel())
	{
	case 1:
	case 2:
	case 3:
	case 4:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		}
	}break;
	case 5:
	case 6:
	{
		int nRandom = myrand(0, 2);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		case 2:
			sSkillID = 109005;
			break;
		}
	}break;
	case 7:
	case 8:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		case 2:
			sSkillID = 109005;
			break;
		case 3:
			sSkillID = 109007;
			break;
		}
	}break;
	case 9:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		case 2:
			sSkillID = 109005;
			break;
		case 3:
			sSkillID = 109007;
			break;
		case 4:
			sSkillID = 109009;
			break;
		case 5:
			sSkillID = 109010;
			break;
		}
	}break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		}
	}break;
	case 15:
	case 16:
	case 17:
	{
		int nRandom = myrand(0, 2);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		}
	}break;
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		}
	}break;
	case 27:
	{
		int nRandom = myrand(0, 4);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		}
	}break;
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	{
		int nRandom = myrand(0, 4);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		}
	}break;
	case 33:
	{
		int nRandom = myrand(0, 5);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		}
	}break;
	case 34:
	case 35:
	{
		int nRandom = myrand(0, 6);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		}
	}break;
	case 36:
	case 37:
	case 38:
	{
		int nRandom = myrand(0, 6);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		}
	}break;
	case 39:
	{
		int nRandom = myrand(0, 7);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		}
	}break;
	case 40:
	case 41:
	{
		int nRandom = myrand(0, 7);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		}
	}break;
	case 42:
	{
		int nRandom = myrand(0, 8);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		}
	}break;
	case 43:
	{
		int nRandom = myrand(0, 9);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		}
	}break;
	case 44:
	case 45:
	{
		int nRandom = myrand(0, 10);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		}
	}break;
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	{
		int nRandom = myrand(0, 10);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		}
	}break;
	case 51:
	{
		int nRandom = myrand(0, 11);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		}
	}break;
	case 52:
	case 53:
	case 54:
	case 55:
	{
		int nRandom = myrand(0, 11);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		}
	}break;
	case 56:
	{
		int nRandom = myrand(0, 12);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		case 12:
			sSkillID = 109556;
			break;
		}
	}break;
	case 57:
	{
		int nRandom = myrand(0, 13);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		case 12:
			sSkillID = 109556;
			break;
		case 13:
			sSkillID = 109557;
			break;
		}
	}break;
	case 58:
	case 59:
	{
		int nRandom = myrand(0, 13);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		case 12:
			sSkillID = 109556;
			break;
		case 13:
			sSkillID = 109557;
			break;
		}
	}break;
	case 60:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110551;
			break;
		case 12:
			sSkillID = 110556;
			break;
		case 13:
			sSkillID = 110557;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110551;
			break;
		case 12:
			sSkillID = 110556;
			break;
		case 13:
			sSkillID = 110557;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 70:
	case 71:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110551;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 72:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110572;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110572;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 80:
	case 81:
	case 82:
	case 83:
	{
		int nRandom = myrand(8, 15);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110572;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		case 15:
			sSkillID = 110575;
			break;
		}
	}break;
	}

	if (GetNation() == ELMORAD)
		sSkillID += 100000;

	if (GetLevel() <= MAX_LEVEL_ARDREAM)
	{
		if (GetNation() == KARUS)
		{
			if (sSkillID > 110000)
				sSkillID -= 1000;
		}
		else
		{
			if (sSkillID > 210000)
				sSkillID -= 1000;
		}
	}

	_MAGIC_TABLE* pSkill = g_pMain->m_MagicTableArray.GetData(sSkillID);
	if (pSkill == nullptr)
		return;

	if (sSkillID != 110002 && sSkillID != 210002)
		sSkillID += 100;

	Unit* pUnit = g_pMain->GetUnitPtr(tid);
	if (pUnit == nullptr
		|| pUnit->isDead()
		|| pUnit->isPlayer() && TO_USER(pUnit)->isGM())
		return;

	float sRange = (float)pSkill->sRange > 0 ? pSkill->sRange : 7.0f;
	float fDis = GetDistanceSqrt(pUnit);
	if (fDis > sRange)
		return;

	if ((uint32)UNIXTIME >= (m_sSkillCoolDown[0]))
	{
		MagicPacket(MAGIC_CASTING, sSkillID, GetID(), pUnit->GetID(), (uint16)pUnit->GetX(), (uint16)pUnit->GetY(), (uint16)pUnit->GetZ());
		MagicPacket(MAGIC_EFFECTING, sSkillID, GetID(), pUnit->GetID(), (uint16)pUnit->GetX(), (uint16)pUnit->GetY(), (uint16)pUnit->GetZ());
		m_sSkillCoolDown[0] = (uint32)UNIXTIME + 3; m_sSpeed = NULL;
	}
}

void CBot::RegionGetLightningMageDamageMagic(int16 tid, float X, float Y, float Z, int16 will_x, int16 will_y, int16 will_z, float sSpeed, int8 echo)
{
	if (tid < 0)
		return;

	uint32 sSkillID = 0;
	switch (GetLevel())
	{
	case 1:
	case 2:
	case 3:
	case 4:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		}
	}break;
	case 5:
	case 6:
	{
		int nRandom = myrand(0, 2);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		case 2:
			sSkillID = 109005;
			break;
		}
	}break;
	case 7:
	case 8:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		case 2:
			sSkillID = 109005;
			break;
		case 3:
			sSkillID = 109007;
			break;
		}
	}break;
	case 9:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109001;
			break;
		case 1:
			sSkillID = 109002;
			break;
		case 2:
			sSkillID = 109005;
			break;
		case 3:
			sSkillID = 109007;
			break;
		case 4:
			sSkillID = 109009;
			break;
		case 5:
			sSkillID = 109010;
			break;
		}
	}break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	{
		int nRandom = myrand(0, 1);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		}
	}break;
	case 15:
	case 16:
	case 17:
	{
		int nRandom = myrand(0, 2);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		}
	}break;
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	{
		int nRandom = myrand(0, 3);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		}
	}break;
	case 27:
	{
		int nRandom = myrand(0, 4);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		}
	}break;
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	{
		int nRandom = myrand(0, 4);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		}
	}break;
	case 33:
	{
		int nRandom = myrand(0, 5);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		}
	}break;
	case 34:
	case 35:
	{
		int nRandom = myrand(0, 6);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		}
	}break;
	case 36:
	case 37:
	case 38:
	{
		int nRandom = myrand(0, 6);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		}
	}break;
	case 39:
	{
		int nRandom = myrand(0, 7);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		}
	}break;
	case 40:
	case 41:
	{
		int nRandom = myrand(0, 7);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		}
	}break;
	case 42:
	{
		int nRandom = myrand(0, 8);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		}
	}break;
	case 43:
	{
		int nRandom = myrand(0, 9);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		}
	}break;
	case 44:
	case 45:
	{
		int nRandom = myrand(0, 10);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		}
	}break;
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	{
		int nRandom = myrand(0, 10);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		}
	}break;
	case 51:
	{
		int nRandom = myrand(0, 11);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		}
	}break;
	case 52:
	case 53:
	case 54:
	case 55:
	{
		int nRandom = myrand(0, 11);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		}
	}break;
	case 56:
	{
		int nRandom = myrand(0, 12);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		case 12:
			sSkillID = 109556;
			break;
		}
	}break;
	case 57:
	{
		int nRandom = myrand(0, 13);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		case 12:
			sSkillID = 109556;
			break;
		case 13:
			sSkillID = 109557;
			break;
		}
	}break;
	case 58:
	case 59:
	{
		int nRandom = myrand(0, 13);
		switch (nRandom)
		{
		case 0:
			sSkillID = 109503;
			break;
		case 1:
			sSkillID = 109509;
			break;
		case 2:
			sSkillID = 109515;
			break;
		case 3:
			sSkillID = 109518;
			break;
		case 4:
			sSkillID = 109527;
			break;
		case 5:
			sSkillID = 109533;
			break;
		case 6:
			sSkillID = 109535;
			break;
		case 7:
			sSkillID = 109539;
			break;
		case 8:
			sSkillID = 109542;
			break;
		case 9:
			sSkillID = 109543;
			break;
		case 10:
			sSkillID = 109545;
			break;
		case 11:
			sSkillID = 109551;
			break;
		case 12:
			sSkillID = 109556;
			break;
		case 13:
			sSkillID = 109557;
			break;
		}
	}break;
	case 60:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110551;
			break;
		case 12:
			sSkillID = 110556;
			break;
		case 13:
			sSkillID = 110557;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110551;
			break;
		case 12:
			sSkillID = 110556;
			break;
		case 13:
			sSkillID = 110557;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 70:
	case 71:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110551;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 72:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110572;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	{
		int nRandom = myrand(8, 14);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110572;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		}
	}break;
	case 80:
	case 81:
	case 82:
	case 83:
	{
		int nRandom = myrand(8, 15);
		switch (nRandom)
		{
		case 8:
			sSkillID = 110542;
			break;
		case 9:
			sSkillID = 110543;
			break;
		case 10:
			sSkillID = 110545;
			break;
		case 11:
			sSkillID = 110572;
			break;
		case 12:
			sSkillID = 110571;
			break;
		case 13:
			sSkillID = 110570;
			break;
		case 14:
			sSkillID = 110560;
			break;
		case 15:
			sSkillID = 110575;
			break;
		}
	}break;
	}

	if (GetNation() == ELMORAD)
		sSkillID += 100000;

	if (GetLevel() <= MAX_LEVEL_ARDREAM)
	{
		if (GetNation() == KARUS)
		{
			if (sSkillID > 110000)
				sSkillID -= 1000;
		}
		else
		{
			if (sSkillID > 210000)
				sSkillID -= 1000;
		}
	}

	_MAGIC_TABLE* pSkill = g_pMain->m_MagicTableArray.GetData(sSkillID);
	if (pSkill == nullptr)
		return;

	if (sSkillID != 110002 && sSkillID != 210002)
		sSkillID += 200;

	Unit* pUnit = g_pMain->GetUnitPtr(tid);
	if (pUnit == nullptr
		|| pUnit->isDead()
		|| pUnit->isPlayer() && TO_USER(pUnit)->isGM())
		return;

	float sRange = (float)pSkill->sRange > 0 ? pSkill->sRange : 7.0f;
	float fDis = GetDistanceSqrt(pUnit);
	if (fDis > sRange)
		return;

	if ((uint32)UNIXTIME >= (m_sSkillCoolDown[0]))
	{
		MagicPacket(MAGIC_CASTING, sSkillID, GetID(), pUnit->GetID(), (uint16)pUnit->GetX(), (uint16)pUnit->GetY(), (uint16)pUnit->GetZ());
		MagicPacket(MAGIC_EFFECTING, sSkillID, GetID(), pUnit->GetID(), (uint16)pUnit->GetX(), (uint16)pUnit->GetY(), (uint16)pUnit->GetZ());
		m_sSkillCoolDown[0] = (uint32)UNIXTIME + 3; m_sSpeed = NULL;
	}
}

void CBot::RegionGetPriestDamageMagic(int16 tid, float X, float Y, float Z, int16 will_x, int16 will_y, int16 will_z, float sSpeed, int8 echo)
{
	if (tid < 0)
		return;

	uint32 sSkillID = 0;
	switch (GetLevel())
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		sSkillID = 101001;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		sSkillID = 111511;
		break;
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
		sSkillID = 111520;
		break;
	case 42:
		sSkillID = 111542;
		break;
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	{
		int nRandom = myrand(1, 2);
		switch (nRandom)
		{
		case 1:
			sSkillID = 111520;
			break;
		case 2:
			sSkillID = 111542;
			break;
		}
	}break;
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	{
		int nRandom = myrand(1, 3);
		switch (nRandom)
		{
		case 1:
			sSkillID = 111520;
			break;
		case 2:
			sSkillID = 111542;
			break;
		case 3:
			sSkillID = 111551;
			break;
		}
	}break;
	case 60:
	case 61:
	{
		int nRandom = myrand(1, 3);
		switch (nRandom)
		{
		case 1:
			sSkillID = 112520;
			break;
		case 2:
			sSkillID = 112542;
			break;
		case 3:
			sSkillID = 112551;
			break;
		}
	}break;
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	case 70:
	case 71:
		sSkillID = 112802;
		break;
	case 72:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 80:
	case 81:
	case 82:
	case 83:
		sSkillID = 112815;
		break;
	}

	if (GetNation() == ELMORAD)
		sSkillID += 100000;

	if (GetLevel() <= MAX_LEVEL_ARDREAM)
	{
		if (GetNation() == KARUS)
		{
			if (sSkillID > 112000)
				sSkillID -= 1000;
		}
		else
		{
			if (sSkillID > 212000)
				sSkillID -= 1000;
		}
	}

	_MAGIC_TABLE* pSkill = g_pMain->m_MagicTableArray.GetData(sSkillID);
	if (pSkill == nullptr)
		return;

	Unit* pUnit = g_pMain->GetUnitPtr(tid);
	if (pUnit == nullptr
		|| pUnit->isDead()
		|| pUnit->isPlayer() && TO_USER(pUnit)->isGM())
		return;

	float sRange = (float)pSkill->sRange > 0 ? pSkill->sRange : 7.0f;
	float fDis = GetDistanceSqrt(pUnit);
	if (fDis > sRange)
		return;

	if ((uint32)UNIXTIME >= (m_sSkillCoolDown[0]))
	{
		MagicPacket(MAGIC_CASTING, sSkillID, GetID(), pUnit->GetID(), (uint16)pUnit->GetX(), (uint16)pUnit->GetY(), (uint16)pUnit->GetZ());
		MagicPacket(MAGIC_EFFECTING, sSkillID, GetID(), pUnit->GetID(), (uint16)pUnit->GetX(), (uint16)pUnit->GetY(), (uint16)pUnit->GetZ());
		m_sSkillCoolDown[0] = (uint32)UNIXTIME + 3; m_sSpeed = NULL;
	}
}