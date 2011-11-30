/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

#include "MoonScriptCreatureAI.h"

#ifndef MOON_SCRIPT_BOSS_AI
#define MOON_SCRIPT_BOSS_AI

class MoonScriptBossAI;

typedef std::pair<int32, SpellDesc*> PhaseSpellPair;
typedef std::vector<PhaseSpellPair> PhaseSpellArray;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class MoonScriptBossAI
class MoonScriptBossAI : public MoonScriptCreatureAI
{
	public:
		MoonScriptBossAI(Creature* pCreature);
		virtual ~MoonScriptBossAI();

		//Basic Interface
		SpellDesc*		AddPhaseSpell(int32 pPhase, SpellDesc* pSpell);
		int32			GetPhase();
		void			SetPhase(int32 pPhase, SpellDesc* pPhaseChangeSpell = NULL);
		void			SetEnrageInfo(SpellDesc* pSpell, int32 pTriggerMilliseconds);

		//Reimplemented Events
		virtual void	OnCombatStart(Unit* pTarget);
		virtual void	OnCombatStop(Unit* pTarget);
		virtual void	AIUpdate();

	protected:
		int32			mPhaseIndex;
		PhaseSpellArray	mPhaseSpells;
		SpellDesc*		mEnrageSpell;
		int32			mEnrageTimerDuration;
		int32			mEnrageTimer;
};

#endif MOON_SCRIPT_BOSS_AI