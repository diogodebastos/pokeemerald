Route201_MapScripts::
	.byte 0

@ Trainers

Route201_EventScript_Marcus::
	trainerbattle_single TRAINER_ROUTE201_MARCUS, Route201_Text_MarcusIntro, Route201_Text_MarcusDefeated
	msgbox Route201_Text_MarcusPostBattle, MSGBOX_AUTOCLOSE
	end

Route201_EventScript_Luna::
	trainerbattle_single TRAINER_ROUTE201_LUNA, Route201_Text_LunaIntro, Route201_Text_LunaDefeated
	msgbox Route201_Text_LunaPostBattle, MSGBOX_AUTOCLOSE
	end

Route201_EventScript_Derek::
	trainerbattle_single TRAINER_ROUTE201_DEREK, Route201_Text_DerekIntro, Route201_Text_DerekDefeated
	msgbox Route201_Text_DerekPostBattle, MSGBOX_AUTOCLOSE
	end

Route201_EventScript_Kai::
	trainerbattle_single TRAINER_ROUTE201_KAI, Route201_Text_KaiIntro, Route201_Text_KaiDefeated
	msgbox Route201_Text_KaiPostBattle, MSGBOX_AUTOCLOSE
	end

Route201_EventScript_Mira::
	trainerbattle_single TRAINER_ROUTE201_MIRA, Route201_Text_MiraIntro, Route201_Text_MiraDefeated
	msgbox Route201_Text_MiraPostBattle, MSGBOX_AUTOCLOSE
	end

@ NPCs

Route201_EventScript_ShrineElder::
	lock
	faceplayer
	msgbox Route201_Text_ShrineElder, MSGBOX_DEFAULT
	release
	end

Route201_EventScript_Woman::
	msgbox Route201_Text_Woman, MSGBOX_NPC
	end

@ Signs

Route201_EventScript_SignOldale::
	msgbox Route201_Text_SignOldale, MSGBOX_SIGN
	end

Route201_EventScript_SignRoute102::
	msgbox Route201_Text_SignRoute102, MSGBOX_SIGN
	end

Route201_EventScript_ShrineSign::
	msgbox Route201_Text_ShrineSign, MSGBOX_SIGN
	end

@ Text - Trainers

Route201_Text_MarcusIntro:
	.string "I've been training on this route\n"
	.string "every day!\p"
	.string "Think you can take me on?$"

Route201_Text_MarcusDefeated:
	.string "Whoa! You're way stronger\n"
	.string "than I expected!$"

Route201_Text_MarcusPostBattle:
	.string "There's something about this route\n"
	.string "that makes POKéMON stronger.\p"
	.string "Maybe it's the shrine?$"

Route201_Text_LunaIntro:
	.string "The flowers here are so pretty!\n"
	.string "Oh, you want to battle?\p"
	.string "Fine, but don't trample\n"
	.string "my flower patch!$"

Route201_Text_LunaDefeated:
	.string "Oh no! My POKéMON!$"

Route201_Text_LunaPostBattle:
	.string "Have you seen the pond nearby?\n"
	.string "Rare POKéMON come to drink\l"
	.string "the water there sometimes.$"

Route201_Text_DerekIntro:
	.string "The grass here is crawling with\n"
	.string "all kinds of bugs!\p"
	.string "I'll show you my collection!$"

Route201_Text_DerekDefeated:
	.string "My bugs!\n"
	.string "They need more training…$"

Route201_Text_DerekPostBattle:
	.string "The tall grass on this route\n"
	.string "has POKéMON you won't find\l"
	.string "anywhere else nearby.\p"
	.string "Keep your eyes peeled!$"

Route201_Text_KaiIntro:
	.string "I saw a strange POKéMON near\n"
	.string "the shrine earlier…\p"
	.string "But first, let's battle!$"

Route201_Text_KaiDefeated:
	.string "Darn! Back to the drawing\n"
	.string "board!$"

Route201_Text_KaiPostBattle:
	.string "The old man at the shrine says\n"
	.string "this route was once a place\l"
	.string "where trainers gathered to\l"
	.string "test their strength.\p"
	.string "I can see why!$"

Route201_Text_MiraIntro:
	.string "The scent of flowers in the air…\n"
	.string "It puts my POKéMON at ease.\p"
	.string "Shall we have a relaxed battle?$"

Route201_Text_MiraDefeated:
	.string "That wasn't very relaxing\n"
	.string "at all!$"

Route201_Text_MiraPostBattle:
	.string "If you walk through the thick\n"
	.string "grass quietly, you might spot\l"
	.string "a RALTS.\p"
	.string "They're very shy, though.$"

@ Text - NPCs

Route201_Text_ShrineElder:
	.string "Ah, a young TRAINER…\p"
	.string "This shrine has stood here since\n"
	.string "ancient times.\p"
	.string "They say the legendary POKéMON\n"
	.string "RAYQUAZA once descended from\l"
	.string "the sky and rested here.\p"
	.string "The air on this route still hums\n"
	.string "with that ancient power.\p"
	.string "Perhaps that's why the POKéMON\n"
	.string "here are a bit… unusual.\p"
	.string "Take care on your journey,\n"
	.string "young one.$"

Route201_Text_Woman:
	.string "I love taking walks on this route.\n"
	.string "The pond is so peaceful.\p"
	.string "Sometimes I see rare POKéMON\n"
	.string "hiding in the tall grass!$"

@ Text - Signs

Route201_Text_SignOldale:
	.string "ROUTE 201\n"
	.string "{RIGHT_ARROW} OLDALE TOWN$"

Route201_Text_SignRoute102:
	.string "ROUTE 201\n"
	.string "{LEFT_ARROW} ROUTE 102$"

Route201_Text_ShrineSign:
	.string "THE ANCIENT SHRINE\p"
	.string "Where sky meets earth,\n"
	.string "great power sleeps.$"
