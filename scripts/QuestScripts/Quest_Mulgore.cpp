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

class The_Plains_Vision : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(The_Plains_Vision);
		The_Plains_Vision(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 2)
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You follow me.");
			if(iWaypointId == 22)
			{
				sEAS.DeleteWaypoints(_unit);
				_unit->Despawn(500, 0);
			}
		}
};

class SkornWhitecloud_Gossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()), plr->GetSession()->language);
			sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), plr, menu);
			if(plr->HasQuest(770))
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Tell me a story, Skorn.", 0);
			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			if(Id == 0)
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 523, plr);
		}
};

class CairneBloodhoof_Gossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()), plr->GetSession()->language);
			sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), plr, menu);
			if(plr->HasQuest(925))
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "I know this is rather silly but a young ward who is a bit shy would like your hoofprint.", 0);
			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			if(Id == 0)
			{
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 7014, plr);
				TO_CREATURE(pObject)->CastSpell(plr, 23123, false);
			}
		}
};

void SetupMulgore(ScriptMgr* mgr)
{
	mgr->register_creature_script(2983, &The_Plains_Vision::Create);
	mgr->register_creature_gossip(3052, new SkornWhitecloud_Gossip);
	mgr->register_creature_gossip(3057, new CairneBloodhoof_Gossip);
}
