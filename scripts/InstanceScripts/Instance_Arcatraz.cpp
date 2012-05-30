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
	
#define	WARDEN_MELLICHAR 0
#define CN_DALLIAH_THE_DOOMSAYER 20885
#define CN_WRATH_SCRYER_SOCCOTHRATES 20886
enum InstanceArcatraz
{
    CONTAINMENT_CORE_SECURITY_FIELD_ALPHA = 184318, //door opened when Wrath-Scryer Soccothrates dies
    CONTAINMENT_CORE_SECURITY_FIELD_BETA  = 184319, //door opened when Dalliah the Doomsayer dies
    POD_ALPHA                             = 183961, //pod first boss wave
    POD_BETA                              = 183963, //pod second boss wave
    POD_DELTA                             = 183964, //pod third boss wave
    POD_GAMMA                             = 183962, //pod fourth boss wave
    POD_OMEGA                             = 183965, //pod fifth boss wave
    WARDENS_SHIELD                        = 184802, // warden shield

    NPC_MELLICHAR						= 20904 //skyriss will kill this unit
};

class ArcatrazInstanceScript : public MoonInstanceScript
{
	public:
		MOONSCRIPT_INSTANCE_FACTORY_FUNCTION(ArcatrazInstanceScript, MoonInstanceScript);
		ArcatrazInstanceScript(MapMgr* pMapMgr) : MoonInstanceScript(pMapMgr) 
		{
			WardenShieldGuid = CoreSecurityAlphaGuid = CoreSecurityBetaGuid = 0;
			for(uint32 i=0; i<=3; i++)
				OrbGuid[i] = 0;
		}

		void OnGameObjectPushToWorld(GameObject* pGameObject)
		{
			switch(pGameObject->GetEntry())
			{
				case CONTAINMENT_CORE_SECURITY_FIELD_ALPHA: CoreSecurityAlphaGuid = pGameObject->GetGUID(); break;
				case CONTAINMENT_CORE_SECURITY_FIELD_BETA: CoreSecurityBetaGuid = pGameObject->GetGUID(); break;
				case WARDENS_SHIELD: WardenShieldGuid = pGameObject->GetGUID(); break;
				case POD_ALPHA: OrbGuid[0] = pGameObject->GetGUID(); break;
				case POD_BETA: OrbGuid[1] = pGameObject->GetGUID(); break;
				case POD_DELTA: OrbGuid[2] = pGameObject->GetGUID(); break;
				case POD_GAMMA: OrbGuid[3] = pGameObject->GetGUID(); break;
			}
		}

		void OnCreatureDeath(Creature* pCreature, Unit* pUnit)
		{		
			switch(pCreature->GetEntry())
			{
				case CN_DALLIAH_THE_DOOMSAYER:
				{
					if(GameObject* pGo = GetGameObjectByGuid(CoreSecurityBetaGuid))
						pGo->SetState(GAMEOBJECT_STATE_OPEN);
				}break;
				case CN_WRATH_SCRYER_SOCCOTHRATES:
				{
					if(GameObject* pGo2 = GetGameObjectByGuid(CoreSecurityAlphaGuid))
						pGo2->SetState(GAMEOBJECT_STATE_OPEN);
				}break;
			}
		}

		uint32 GetInstanceData(uint32 pType, uint32 pIndex)
		{
			return mEncounters[pIndex];
		}

		void SetInstanceData(uint32 pType, uint32 pIndex, uint32 pData)
		{
			if(pIndex == WARDEN_MELLICHAR)
			{
				switch(pData)
				{
					case 1:	//agroo
					{
						if(GameObject* pGo = GetGameObjectByGuid(WardenShieldGuid))
							pGo->SetState(GAMEOBJECT_STATE_OPEN);
					}break;
					case 2:	//first orb
					{
						if(GameObject* pGo = GetGameObjectByGuid(OrbGuid[0]))
							pGo->SetState(GAMEOBJECT_STATE_OPEN);
					}break;
					case 3:	//second orb
					{
						if(GameObject* pGo = GetGameObjectByGuid(OrbGuid[1]))
							pGo->SetState(GAMEOBJECT_STATE_OPEN);
					}break;
					case 4:	//third orb
					{
						if(GameObject* pGo = GetGameObjectByGuid(OrbGuid[2]))
							pGo->SetState(GAMEOBJECT_STATE_OPEN);
					}break;
					case 5:	//four orb
					{
						if(GameObject* pGo = GetGameObjectByGuid(OrbGuid[3]))
							pGo->SetState(GAMEOBJECT_STATE_OPEN);
					}break;
				}
			}
		}

	private:
		uint64 WardenShieldGuid, CoreSecurityAlphaGuid, CoreSecurityBetaGuid, OrbGuid[3], mEncounters[1];
};

// Zereketh the UnboundAI
#define CN_ZEREKETH			20870
#define CN_VOIDZONEARC		21101

#define SEED_OF_C			36123
#define SHADOW_NOVA			36127
#define CONSUMPTION			30498
#define VOID_ZONE 36119

class ZerekethAI : public CreatureAI
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ZerekethAI)
		ZerekethAI(Creature* pCreature) : CreatureAI(pCreature)
		{
			SpeechTimer = (40+rand()%11) * 1000;
			SeedTimer		= (14+rand()%14)*1000;
			ShadowNovaTimer	= (15+rand()%6)*1000;
			VoidZoneTimer	= (21+rand()%12)*1000;
		}

		void OnCombatStart(Unit* mTarget)
		{
			Emote(_unit, "Life energy to... consume.", Text_Yell, 11250);
		}

		void OnTargetDied(Unit * pUnit)
		{
			if(rand()%2)
				Emote(_unit, "This vessel...is empty.", Text_Yell, 11251);
			else
				Emote(_unit, "No... more... life.", Text_Yell, 11252);
		}

		void OnDied(Unit * mKiller)
		{
			Emote(_unit, "The Void... beckons.", Text_Yell, 11255);
		}

		void AIUpdate()
		{
			if(SeedTimer < mAIUpdateFrequency)
			{
				if(Unit * pTarget = GetTarget(TARGET_RANDOM_SINGLE))
					_unit->CastSpell(pTarget, dbcSpell.LookupEntry(VOID_ZONE), true);

				SeedTimer = (14+rand()%8)*1000;
			}
			else
				SeedTimer -= mAIUpdateFrequency;

			if(VoidZoneTimer < mAIUpdateFrequency)
			{
				if(Unit * pTarget = GetTarget(TARGET_RANDOM_SINGLE))
					_unit->CastSpell(pTarget, dbcSpell.LookupEntry(VOID_ZONE), true);

				VoidZoneTimer = (32+rand()%14)*1000;
			}
			else
				VoidZoneTimer -= mAIUpdateFrequency;

			if(VoidZoneTimer -= mAIUpdateFrequency)
			{
				if(rand()%2)
					Emote(_unit, "The shadow... will engulf you.", Text_Yell, 11253);
				else
					Emote(_unit, "Darkness...consumes...all.", Text_Yell, 11254);

				if(Unit * pTarget = GetTarget(TARGET_ATTACKING))
					_unit->CastSpell(pTarget, dbcSpell.LookupEntry(SHADOW_NOVA), true);

				ShadowNovaTimer = (35+rand()%11)*1000;
			}
			else
				ShadowNovaTimer -= mAIUpdateFrequency;

			if(SpeechTimer < mAIUpdateFrequency)
			{
				if(rand()%2 == 1)
					Emote(_unit, "The shadow... will engulf you.", Text_Yell, 11253);
				else 
					Emote(_unit, "Darkness... consumes all.", Text_Yell, 11254);

				SpeechTimer = (40+rand()%11) * 1000;
			}
			else 
				SpeechTimer -= mAIUpdateFrequency;
		}

	protected:
		uint32 SpeechTimer;
		uint32 SeedTimer;
		uint32 ShadowNovaTimer;
		uint32 VoidZoneTimer;
};

class VoidZoneARC : public CreatureAI
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(VoidZoneARC)
		VoidZoneARC(Creature* pCreature) : CreatureAI(pCreature)
		{
			_unit->Unroot();
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			RegisterAIUpdateEvent(1000);
		}

		void AIUpdate()
		{
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(CONSUMPTION), true);
			RemoveAIUpdateEvent();
		}
};

// Dalliah the DoomsayerAI
#define GIFT_OF_THE_DOOMSAYER 36173
#define WHIRLWIND 36175
#define HEAL 36144
#define SHADOW_WAVE 39016	// Heroic mode spell

class DalliahTheDoomsayerAI : public CreatureAI
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(DalliahTheDoomsayerAI)
		DalliahTheDoomsayerAI(Creature* pCreature) : CreatureAI(pCreature)
		{
			AddSpell(GIFT_OF_THE_DOOMSAYER, Target_Current, 8.0f, 0.0f, -1);

			SpellDesc* WhirlTemp = AddSpell(WHIRLWIND, Target_Self, 15.0f, 0.0f, -1);
			WhirlTemp->AddEmote("Reap the Whirlwind!", Text_Yell, 11089);
			WhirlTemp->AddEmote("I'll cut you to peices!", Text_Yell, 11090);

			SpellDesc* HealTemp = AddSpell(HEAL, Target_Self, 8.0f, 0, -1);
			HealTemp->AddEmote("That is much better.", Text_Yell, 11091);
			HealTemp->AddEmote("Ah, just what I needed.", Text_Yell, 11092);

			if(IsHeroic())
				AddSpell(SHADOW_WAVE, Target_Current, 8.0f, 0, -1);
		}

		void OnCombatStart(Unit* pUnit)
		{
			Emote(_unit, "It is unwise to anger me.", Text_Yell, 11086);
		}

		void OnTargetDied(Unit* pUnit)
		{
			if(rand()%2)
				Emote(_unit, "Completely ineffective! Just like someone else I know!", Text_Yell, 11087);
			else
				Emote(_unit, "You chose the wrong opponent!", Text_Yell, 11088);
		}

		void OnDied(Unit* pUnit)
		{
			Emote(_unit, "Now I'm really... angry...", Text_Yell, 11093);
		}
};

// Wrath-Scryer SoccothratesAI
#define IMMOLATION HeroicInt(39007, 36051)
#define FELFIRE_SHOCK HeroicInt(39006, 35759)
#define FELFIRE_LINE_UP 35770
#define KNOCK_AWAY 20686
#define CHARGE 35754

class WrathScryerSoccothratesAI : public MoonScriptCreatureAI
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WrathScryerSoccothratesAI)
	WrathScryerSoccothratesAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddEmote(Event_OnCombatStart, "At last, a target for my frustrations!", Text_Yell, 11238);
		AddEmote(Event_OnTargetDied, "Yes, that was quiet... satisfying.", Text_Yell, 11239);
		AddEmote(Event_OnTargetDied, "Ha! Much better!", Text_Yell, 11240);
		AddEmote(Event_OnDied, "Knew this was... the only way out.", Text_Yell, 11243);

		AddSpell(IMMOLATION, Target_Self, 10.0f, 0, -1);
		AddSpell(FELFIRE_SHOCK, Target_Current, 8.0f, 0, -1);
		AddSpell(FELFIRE_LINE_UP, Target_Self, 8.0f, 0, -1);
		AddSpell(KNOCK_AWAY, Target_Destination, 6.0f, 0, -1);
		AddSpell(CHARGE, Target_Current, 4.0f, 0, -1);
	}
};

// Harbinger SkyrissAI
// Full event must be scripted for this guy.
#define CN_HARBRINGER_SKYRISS 20912

#define MIND_REND 36924 // DBC: 36859, 36924;
#define FEAR 39415
#define DOMINATION 37162
#define SUMMON_ILLUSION_66 36931	// those 2 don't work
#define SUMMON_ILLUSION_33 36932
// BLINK_VISUAL 36937 ?
// SIMPLE_TELEPORT 12980 ?
// Add sounds related to his dialog with mind controlled guy

class HarbringerSkyrissAI : public MoonScriptCreatureAI
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HarbringerSkyrissAI)
		HarbringerSkyrissAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddEmote(Event_OnCombatStart, "Bear witness to the agent of your demise!", Text_Yell, 11123);
			AddEmote(Event_OnTargetDied, "Your fate is written!", Text_Yell, 11124);
			AddEmote(Event_OnTargetDied, "The chaos I have sown here is but a taste....", Text_Yell, 11125);
			AddEmote(Event_OnDied, "I am merely one of... infinite multitudes.", Text_Yell, 11126);

			AddSpell(MIND_REND, Target_Current, 15.0f, 0, -1);
			SpellDesc* Fear = AddSpell(FEAR, Target_Current, 8.0f, 0, -1);
				Fear->AddEmote("Flee in terror!", Text_Yell, 11129);
				Fear->AddEmote("I will show you horrors undreamed of.", Text_Yell, 11130);

			SpellDesc* Domination = AddSpell(DOMINATION, Target_Current, 6.0f, 0, -1);
				Domination->AddEmote("You will do my bidding, weakling.", Text_Yell, 11127);
				Domination->AddEmote("Your will is no longer your own.", Text_Yell, 11128);
		}

		void AIUpdate()
		{
			uint32 IllusionCount = 0;
			if(GetHealthPercent() <= 66 && IllusionCount == 0)
			{
				_unit->CastSpell(_unit, SUMMON_ILLUSION_66, true);
				_unit->PlaySoundToSet(11131);
				IllusionCount++;
			}
			else if(GetHealthPercent() <= 33 && IllusionCount == 1)
			{
				_unit->CastSpell(_unit, SUMMON_ILLUSION_33, true);
				_unit->PlaySoundToSet(11131);
				IllusionCount++;
			}
		}
};


enum eWardenSpells
{
    SPELL_TARGET_ALPHA  = 36856,
    SPELL_TARGET_BETA   = 36854,
    SPELL_TARGET_DELTA  = 36857,
    SPELL_TARGET_GAMMA  = 36858,
    SPELL_TARGET_OMEGA  = 36852,
    SPELL_BUBBLE_VISUAL = 36849
};

enum eWardenUnits
{
    //phase 2(acid mobs)
    ENTRY_TRICKSTER    = 20905,
    ENTRY_PH_HUNTER    = 20906,
    //phase 3
    ENTRY_MILLHOUSE    = 20977,
    //phase 4(acid mobs)
    ENTRY_AKKIRIS      = 20908,
    ENTRY_SULFURON     = 20909,
    //phase 5(acid mobs)
    ENTRY_TW_DRAK      = 20910,
    ENTRY_BL_DRAK      = 20911,
    //phase 6
    NPC_SKYRISS      = 20912
};

#define CN_WARDEN_MELLICHAR	20904


static Location pSummonCoords[5]=
{
    {478.326f, -148.505f, 42.56f, 3.19f},                   // Trickster or Phase Hunter
    {413.292f, -148.378f, 42.56f, 6.27f},                   // Millhouse
    {420.179f, -174.396f, 42.58f, 0.02f},                   // Akkiris or Sulfuron
    {471.795f, -174.58f, 42.58f, 3.06f},                    // Twilight or Blackwing Drakonaar
    {445.763f, -191.639f, 44.64f, 1.60f}                    // Skyriss
};

class WardenMellicharAI : public MoonScriptBossAI
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(WardenMellicharAI)
		WardenMellicharAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
		{
			SetCanMove(false);
			mInstance = GetInstanceScript();
			SetPhase(0);
		}

		void OnCombatStart(Unit* mTarget)
		{
			SetCanEnterCombat(false);
			SetCanMove(false);
			SetAllowMelee(false);
			SetAllowSpell(false);

			Phase_Timer = AddTimer(22000);

			Emote("I knew the prince would be angry but, I... I have not been myself. I had to let them out! The great one speaks to me, you see. Wait--outsiders. Kael'thas did not send you! Good... I'll just tell the prince you released the prisoners!",
			Text_Yell, 11222);
			_unit->CastSpell(_unit, SPELL_BUBBLE_VISUAL, true);
			mInstance->SetInstanceData(Data_UnspecifiedType, WARDEN_MELLICHAR, 1);
		}


		void AIUpdate()
		{
			// ORB ONE
			if(GetPhase() == 0)
			{
				Emote("The naaru kept some of the most dangerous beings in existence here in these cells. Let me introduce you to another...", Text_Yell, 11223);
				_unit->CastSpell(_unit, SPELL_TARGET_ALPHA, false);
				mInstance->SetInstanceData(Data_UnspecifiedType, WARDEN_MELLICHAR, 2);

				if(!HasSummonedNpc && IsTimerFinished(Phase_Timer))
				{
					pSummon = SpawnCreature(rand()%1 ? ENTRY_TRICKSTER : ENTRY_PH_HUNTER, pSummonCoords[0].x, pSummonCoords[0].y, pSummonCoords[0].z, pSummonCoords[0].o);
					HasSummonedNpc = true;
				}

				if(!pSummon->IsAlive() && pSummon != NULL && HasSummonedNpc)
				{
					Emote("Yes, yes... another! Your will is mine! Behold another terrifying creature of incomprehensible power!", Text_Yell, 11224);
					_unit->CastSpell(_unit, SPELL_TARGET_BETA, false);
					mInstance->SetInstanceData(Data_UnspecifiedType, WARDEN_MELLICHAR, 3);
					ResetTimer(Phase_Timer, 6000);
					SetPhase(1);
					HasSummonedNpc = false;
					pSummon = NULL;
				}

			//ORB TWO
			}else if(IsTimerFinished(Phase_Timer) && GetPhase() == 1)
			{
				Millhouse = _unit->GetMapMgr()->GetInterface()->SpawnCreature(ENTRY_MILLHOUSE, pSummonCoords[1].x, pSummonCoords[1].y, pSummonCoords[1].z, pSummonCoords[1].o, false, true, 0, 0);
				if(Millhouse != NULL)
				{
					Millhouse->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Where in Bonzo's brass buttons am I? And who are-- yaaghh, that's one mother of a headache!", 2000);
					sEventMgr.AddEvent(TO_OBJECT(Millhouse), &Object::PlaySoundToSet, (uint32)11171, EVENT_UNK, 2000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What is this? A lowly gnome? I will do better, oh great one.", 13000);
					sEventMgr.AddEvent(TO_OBJECT(_unit), &Object::PlaySoundToSet, (uint32)11226, EVENT_UNK, 13000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					
					SetPhase(2);	//starts another after his talk
					ResetTimer(Phase_Timer, 6000);
					_unit->CastSpell(_unit, SPELL_TARGET_BETA, false);
					mInstance->SetInstanceData(Data_UnspecifiedType, WARDEN_MELLICHAR, 4);

					Millhouse->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lowly? Nobody refers to the mighty Millhouse Manastorm as lowly! I have no idea what goes on here, but I will gladly join your fight against this impudent imbecile!", 22000);
					sEventMgr.AddEvent(TO_OBJECT(Millhouse), &Object::PlaySoundToSet, (uint32)11172, EVENT_UNK, 22000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				}
				
			// ORB THREE
			}else if(IsTimerFinished(Phase_Timer) && GetPhase() == 2)
			{
				if(!HasSummonedNpc)
				{
					pSummon = SpawnCreature(rand()%1 ? ENTRY_AKKIRIS : ENTRY_SULFURON, pSummonCoords[2].x, pSummonCoords[2].y, pSummonCoords[2].z, pSummonCoords[2].o);
					HasSummonedNpc = true;
				}

				if(!pSummon->IsAlive() && pSummon != NULL)
				{
					Emote("Anarchy! Bedlam! Oh, you are so wise! Yes, I see it now, of course!", Text_Yell, 11227);
					_unit->CastSpell(_unit, SPELL_TARGET_GAMMA, false);
					mInstance->SetInstanceData(Data_UnspecifiedType, WARDEN_MELLICHAR, 5);
					SetPhase(3);
					ResetTimer(Phase_Timer, 6000);
					pSummon = NULL;
					HasSummonedNpc = false;
				}

			// ORB FOUR
			}else if(IsTimerFinished(Phase_Timer) && GetPhase() == 3)
			{
				if(!HasSummonedNpc)
				{
					pSummon = SpawnCreature(rand()%1 ? ENTRY_TW_DRAK : ENTRY_BL_DRAK, pSummonCoords[3].x, pSummonCoords[3].y, pSummonCoords[3].z, pSummonCoords[3].o);
					HasSummonedNpc = true;
				}
				
				if(!pSummon->IsAlive() && pSummon != NULL)
				{
					mInstance->SetInstanceData(Data_UnspecifiedType, WARDEN_MELLICHAR, 6);
					SetPhase(4);
					ResetTimer(Phase_Timer, 6000);
					pSummon = NULL;
					HasSummonedNpc = false;
				}
			//SKYRISS encounter
			}else if(IsTimerFinished(Phase_Timer) && GetPhase() == 4)
			{
				pSummon = SpawnCreature(NPC_SKYRISS, pSummonCoords[4].x, pSummonCoords[4].y, pSummonCoords[4].z, pSummonCoords[4].o);
				if(pSummon!=NULL)
					Emote("Yes, O great one, right away!", Text_Yell, 11228);
			}
	}

	protected:
		bool HasSummonedNpc;
		uint32 Phasepart, NPC_ID_Spawn, Spawncounter;
		int32 Phase_Timer;
		MoonInstanceScript* mInstance;
		MoonScriptCreatureAI* pSummon;
		Creature* Millhouse;
};

void SetupArcatraz(ScriptMgr* mgr)
{
	mgr->register_instance_script(552, &ArcatrazInstanceScript::Create);
	mgr->register_creature_script(CN_ZEREKETH, &ZerekethAI::Create);
	mgr->register_creature_script(CN_VOIDZONEARC, &VoidZoneARC::Create);

	mgr->register_creature_script(CN_DALLIAH_THE_DOOMSAYER, &DalliahTheDoomsayerAI::Create);
	mgr->register_creature_script(CN_WRATH_SCRYER_SOCCOTHRATES, &WrathScryerSoccothratesAI::Create);
	mgr->register_creature_script(NPC_SKYRISS, &HarbringerSkyrissAI::Create);
	mgr->register_creature_script(NPC_MELLICHAR, &WardenMellicharAI::Create);
}
