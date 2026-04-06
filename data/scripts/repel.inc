EventScript_RepelWoreOff::
	msgbox Text_RepelWoreOff, MSGBOX_DEFAULT
	copyvar VAR_0x8004, VAR_LAST_REPEL_ITEM_ID
	goto_if_eq VAR_0x8004, 0, EventScript_RepelWoreOff_End
	checkitem VAR_0x8004, 1
	goto_if_eq VAR_RESULT, FALSE, EventScript_RepelWoreOff_End
	bufferitemname STR_VAR_1, VAR_0x8004
	msgbox Text_UseAnotherRepel, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_UseAnotherRepel
	closemessage
EventScript_RepelWoreOff_End:
	closemessage
	end

EventScript_UseAnotherRepel:
	special Special_UseRepelFromBag
	goto_if_eq VAR_RESULT, TRUE, EventScript_RepelUsedAnother
	msgbox Text_DontHaveAnotherRepel, MSGBOX_SIGN
	end

EventScript_RepelUsedAnother:
	msgbox Text_UsedAnotherRepel, MSGBOX_SIGN
	end

Text_RepelWoreOff:
	.string "REPEL's effect wore off…$"

Text_UseAnotherRepel:
	.string "Use another {STR_VAR_1}?$"

Text_UsedAnotherRepel:
	.string "{PLAYER} used a {STR_VAR_1}.$"

Text_DontHaveAnotherRepel:
	.string "You don't have another one…$"
