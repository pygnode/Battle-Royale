AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/battle_royale/battle_royale.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/battle_royale_npcs/battle_royale_npcs.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/battle_royale_rewards/battle_royale_rewards.cpp")
AC_ADD_SCRIPT_LOADER("Battle_Royale" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/battle_royale.conf.dist")
