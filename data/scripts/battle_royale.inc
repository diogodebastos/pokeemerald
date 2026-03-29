EventScript_BattleRoyaleVictory::
	lockall
	playfanfare MUS_LEVEL_UP
	waitfanfare
	msgbox Text_BattleRoyaleComplete, MSGBOX_DEFAULT
	closemessage
	dofieldeffect FLDEFF_HALL_OF_FAME_RECORD
	waitfieldeffect FLDEFF_HALL_OF_FAME_RECORD
	delay 40
	special GameClear
	waitstate
	releaseall
	end

Text_BattleRoyaleComplete:
	.string "You defeated every TRAINER in HOENN!\p"
	.string "Your name and your partners will be\n"
	.string "recorded for all time!$"

BattleRoyale_Text_GenericIntro:
	.string "I've been waiting for a challenger!$"

BattleRoyale_Text_GenericDefeated:
	.string "You really are something…$"

BattleRoyale_Text_GenericPostBattle:
	.string "I won't forget this battle.$"

BattleRoyale_Text_BrandonIntro:
	.string "I am BRANDON, PYRAMID KING!\p"
	.string "I have traveled here to test\n"
	.string "the strength of HOENN's trainers!$"

BattleRoyale_Text_BrandonDefeated:
	.string "Superb!\p"
	.string "Your strength is the real thing!$"

BattleRoyale_Text_BrandonPostBattle:
	.string "You have bested the PYRAMID KING.\p"
	.string "I look forward to our next\n"
	.string "encounter!$"

LittlerootTown_Text_RedIntro:
	.string "...$"

LittlerootTown_Text_RedDefeated:
	.string "...$"

LittlerootTown_Text_RedPostBattle:
	.string "... ... ...$"

LittlerootTown_Text_LeafIntro:
	.string "I raised these POKéMON from\n"
	.string "JOHTO all by myself!\p"
	.string "Think you can handle them?$"

LittlerootTown_Text_LeafDefeated:
	.string "No way! My partners...$"

LittlerootTown_Text_LeafPostBattle:
	.string "You're really something.\p"
	.string "RED was right about you.$"
