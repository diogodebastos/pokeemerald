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
