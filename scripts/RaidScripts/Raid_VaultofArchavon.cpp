/*
 * ArcScripts2 Scripts for Arcemu MMORPG Server
 * Copyright (C) 2011-2012 ArcScripts2 team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "Setup.h"

//Archavon
#define	NPC_ARCHAVON			31125
#define SPELL_ROCK_SHARDS_LEFT	58678
#define	SPELL_CRUSHING_LEAP		Raid25manInst(58963, 60895)
#define	SPELL_STOMP				Raid25manInst(58663, 60880)
#define SPELL_IMPALE			Raid25manInst(58666, 60882)
#define	SPELL_CHOCKING_CLOUD	Raid25manInst(58965, 61672)
#define	SPELL_ARCHAVON_BERSERK	47008

class ArchavonAI : public MoonScriptBossAI
{
	public:
		MOONSCRIPT_FACTORY_FUNCTION(ArchavonAI, MoonScriptBossAI);
		ArchavonAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
		{
			mStompTimer = INVALIDATE_TIMER;
			SetEnrageInfo(AddSpell(SPELL_ARCHAVON_BERSERK, Target_Self, 0.0f), MINUTE*5*SEC_IN_MS);
		
			AddSpell(SPELL_ROCK_SHARDS, Target_Self, 20.0f);
			AddSpell(SPELL_CRUSHING_LEAP, Target_RandomPlayer, 30.0f);
			AddSpell(SPELL_CHOCKING_CLOUD, Target_RandomPlayer, 30.0f);
		}

		void OnCombatStart(Unit *mTarget)
		{
			mStompTimer = AddTimer(45*SEC_IN_MS);
			mRockShardsTimer = AddTimer(15*SEC_IN_MS);
			ParentClass::OnCombatStart(mTarget);
		}

		void AIUpdate()
		{
			if(IsTimerFinished(mStompTimer))
			{
				_unit->CastSpell(_unit, SPELL_STOMP, false);
				_unit->CastSpell(_unit, SPELL_IMPALE_STUN, false);
				_unit->CastSpell(_unit, SPELL_IMPALE, false);
				ResetTimer(mStompTimer, 45*SEC_IN_MS);
			}
			ParentClass::AIUpdate();
		}

	private:
		SpellDesc * sRockShardsLeft, *sRockShardsRight;
		int32 mStompTimer;
};

void SetupVaultofArchavon(ScriptMgr * mgr)
{
	mgr->register_creature_script(NPC_ARCHAVON, &ArchavonAI::Create);
}
