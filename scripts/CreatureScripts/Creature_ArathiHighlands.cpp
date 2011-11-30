/*
 * WhyScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2010 WhyDB Team <http://www.whydb.org/>
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

class ProfessorPhizzlethorpe : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ProfessorPhizzlethorpe)
	ProfessorPhizzlethorpe(Creature *pCreature) : CreatureAIScript(pCreature) {}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if( iWaypointId == 15 )
		{
			GetUnit()->GetAIInterface()->deleteWaypoints();
			GetUnit()->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Thanks, I found the fact that, it searched" );
			GetUnit()->Despawn(DEFAULT_DESPAWN_TIMER, 1000);

			if( GetUnit()->m_escorter != NULL )
			{
				GetUnit()->m_escorter->GetQuestLogForEntry(665)->SendQuestComplete();
				GetUnit()->m_escorter = NULL;
			}
		}
	}
};

void SetupArathiHighlandsCreature(ScriptMgr * mgr)
{
	mgr->register_creature_script( 2768,  &ProfessorPhizzlethorpe::Create );	// Professor Phizzlethorpe
}
