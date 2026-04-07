EverGrandeCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, EverGrandeCity_OnTransition
	.byte 0

EverGrandeCity_OnTransition:
	call_if_set FLAG_SYS_WEATHER_CTRL, Common_EventScript_SetAbnormalWeather
	call Common_EventScript_SetupRivalGfxId
	end

EverGrandeCity_EventScript_VictoryRoadSign::
	msgbox EverGrandeCity_Text_EnteringVictoryRoad, MSGBOX_SIGN
	end

EverGrandeCity_EventScript_CitySign::
	msgbox EverGrandeCity_Text_CitySign, MSGBOX_SIGN
	end

EverGrandeCity_EventScript_PokemonLeagueSign::
	msgbox EverGrandeCity_Text_EnteringPokemonLeague, MSGBOX_SIGN
	end

EverGrandeCity_EventScript_SetVisitedEverGrande::
	setflag FLAG_VISITED_EVER_GRANDE_CITY
	setvar VAR_TEMP_1, 1
	end

EverGrandeCity_Text_EnteringVictoryRoad:
	.string "ENTERING VICTORY ROAD$"

EverGrandeCity_Text_EnteringPokemonLeague:
	.string "ENTERING POKéMON LEAGUE\n"
	.string "CENTER GATE$"

EverGrandeCity_Text_CitySign:
	.string "EVER GRANDE CITY\p"
	.string "“The paradise of flowers, the sea,\n"
	.string "and POKéMON.”$"


EverGrandeCity_EventScript_FrontierRaul::
	trainerbattle_single TRAINER_FRONTIER_RAUL, BattleRoyale_Text_GenericIntro, BattleRoyale_Text_GenericDefeated
	msgbox BattleRoyale_Text_GenericPostBattle, MSGBOX_AUTOCLOSE
	end

@ =============================================
@ Ever Grande City - Final Rival Battle
@ =============================================

EverGrandeCity_EventScript_Rival::
	lock
	faceplayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EverGrandeCity_EventScript_RivalMay
	goto_if_eq VAR_RESULT, FEMALE, EverGrandeCity_EventScript_RivalBrendan
	end

@ --- May (player is male) ---

EverGrandeCity_EventScript_RivalMay::
	playbgm MUS_ENCOUNTER_MAY, TRUE
	msgbox EverGrandeCity_Text_MayBeforeBattle, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, EverGrandeCity_EventScript_BattleMayTreecko
	case 1, EverGrandeCity_EventScript_BattleMayTorchic
	case 2, EverGrandeCity_EventScript_BattleMayMudkip
	end

EverGrandeCity_EventScript_BattleMayTreecko::
	trainerbattle_no_intro TRAINER_MAY_EVERGRANDE_TREECKO, EverGrandeCity_Text_MayDefeat
	goto EverGrandeCity_EventScript_DefeatedMay
	end

EverGrandeCity_EventScript_BattleMayTorchic::
	trainerbattle_no_intro TRAINER_MAY_EVERGRANDE_TORCHIC, EverGrandeCity_Text_MayDefeat
	goto EverGrandeCity_EventScript_DefeatedMay
	end

EverGrandeCity_EventScript_BattleMayMudkip::
	trainerbattle_no_intro TRAINER_MAY_EVERGRANDE_MUDKIP, EverGrandeCity_Text_MayDefeat
	goto EverGrandeCity_EventScript_DefeatedMay
	end

EverGrandeCity_EventScript_DefeatedMay::
	msgbox EverGrandeCity_Text_MayAfterBattle, MSGBOX_DEFAULT
	goto EverGrandeCity_EventScript_RivalFlyAway
	end

@ --- Brendan (player is female) ---

EverGrandeCity_EventScript_RivalBrendan::
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	msgbox EverGrandeCity_Text_BrendanBeforeBattle, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, EverGrandeCity_EventScript_BattleBrendanTreecko
	case 1, EverGrandeCity_EventScript_BattleBrendanTorchic
	case 2, EverGrandeCity_EventScript_BattleBrendanMudkip
	end

EverGrandeCity_EventScript_BattleBrendanTreecko::
	trainerbattle_no_intro TRAINER_BRENDAN_EVERGRANDE_TREECKO, EverGrandeCity_Text_BrendanDefeat
	goto EverGrandeCity_EventScript_DefeatedBrendan
	end

EverGrandeCity_EventScript_BattleBrendanTorchic::
	trainerbattle_no_intro TRAINER_BRENDAN_EVERGRANDE_TORCHIC, EverGrandeCity_Text_BrendanDefeat
	goto EverGrandeCity_EventScript_DefeatedBrendan
	end

EverGrandeCity_EventScript_BattleBrendanMudkip::
	trainerbattle_no_intro TRAINER_BRENDAN_EVERGRANDE_MUDKIP, EverGrandeCity_Text_BrendanDefeat
	goto EverGrandeCity_EventScript_DefeatedBrendan
	end

EverGrandeCity_EventScript_DefeatedBrendan::
	msgbox EverGrandeCity_Text_BrendanAfterBattle, MSGBOX_DEFAULT
	goto EverGrandeCity_EventScript_RivalFlyAway
	end

@ --- Rival fly away ---

EverGrandeCity_EventScript_RivalFlyAway::
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 50
	setfieldeffectargument 0, 1
	dofieldeffect FLDEFF_NPCFLY_OUT
	delay 15
	removeobject VAR_LAST_TALKED
	waitfieldeffect FLDEFF_NPCFLY_OUT
	savebgm MUS_DUMMY
	fadedefaultbgm
	setflag FLAG_MET_RIVAL_EVERGRANDE
	setflag FLAG_HIDE_EVER_GRANDE_CITY_RIVAL
	release
	end

@ =============================================
@ Rival dialogue text
@ =============================================

EverGrandeCity_Text_MayBeforeBattle:
	.string "MAY: {PLAYER}! I knew I'd find\n"
	.string "you here.\p"
	.string "The POKéMON LEAGUE, huh?\n"
	.string "You've come so far!\p"
	.string "Before you go in there, I want\n"
	.string "one last battle with you.\p"
	.string "I've been training my team just\n"
	.string "for this moment!\p"
	.string "Let's give it everything we've\n"
	.string "got, {PLAYER}!$"

EverGrandeCity_Text_MayDefeat:
	.string "Wow, {PLAYER}...\n"
	.string "You really are something else.$"

EverGrandeCity_Text_MayAfterBattle:
	.string "MAY: That was an amazing battle.\n"
	.string "I'm glad we got to have it.\p"
	.string "You know, battling you has made\n"
	.string "me realize something.\p"
	.string "I don't think battling is really\n"
	.string "my path, you know?\p"
	.string "I want to follow my dad's\n"
	.string "footsteps as a researcher.\p"
	.string "Studying POKéMON, learning how\n"
	.string "they live, caring for them...\p"
	.string "That's what makes me happiest.\p"
	.string "So this is my last battle.\n"
	.string "But I'll be cheering for you!\p"
	.string "Go beat the POKéMON LEAGUE,\n"
	.string "{PLAYER}! Good luck!$"

EverGrandeCity_Text_BrendanBeforeBattle:
	.string "BRENDAN: Hey, {PLAYER}!\n"
	.string "I had a feeling you'd be here.\p"
	.string "So you're really going to\n"
	.string "challenge the POKéMON LEAGUE?\p"
	.string "Before you do, how about one\n"
	.string "last battle between us?\p"
	.string "I've put together my strongest\n"
	.string "team for this. No holding back!\p"
	.string "Let's do this, {PLAYER}!$"

EverGrandeCity_Text_BrendanDefeat:
	.string "Haha, I knew it...\n"
	.string "You're on a whole other level.$"

EverGrandeCity_Text_BrendanAfterBattle:
	.string "BRENDAN: That was one heck of\n"
	.string "a battle. Thanks, {PLAYER}.\p"
	.string "You know, fighting you all\n"
	.string "this time has taught me a lot.\p"
	.string "But I've been thinking...\n"
	.string "battling isn't really my thing.\p"
	.string "I want to be a POKéMON\n"
	.string "researcher like my dad.\p"
	.string "Learning about POKéMON,\n"
	.string "helping them grow, caring\l"
	.string "for them... that's my calling.\p"
	.string "So this was my last battle.\n"
	.string "No regrets, though!\p"
	.string "Now go crush the ELITE FOUR,\n"
	.string "{PLAYER}! I'll be rooting for you!$"

@ =============================================
@ Grinding Nurse — always-rematchable EXP trainer
@ =============================================

EverGrandeCity_EventScript_GrindingNurse::
	lock
	faceplayer
	cleartrainerflag TRAINER_GRINDING_NURSE
	trainerbattle_single TRAINER_GRINDING_NURSE, EverGrandeCity_Text_NurseIntro, EverGrandeCity_Text_NurseDefeat, EverGrandeCity_EventScript_NursePostBattle
	release
	end

EverGrandeCity_EventScript_NursePostBattle::
	msgbox EverGrandeCity_Text_NurseReady, MSGBOX_DEFAULT
	release
	end

EverGrandeCity_Text_NurseIntro:
	.string "I'm here to help you train!\n"
	.string "My BLISSEY team is ready!$"

EverGrandeCity_Text_NurseDefeat:
	.string "Great work! Your POKéMON are\n"
	.string "getting stronger!$"

EverGrandeCity_Text_NurseReady:
	.string "Your POKéMON are looking good!\n"
	.string "I'm ready whenever you are!$"
