#include "../../include/types.h"
#include "../../include/config.h"
#include "../../include/battle.h"
#include "../../include/item.h"
#include "../../include/mega.h"
#include "../../include/pokemon.h"
#include "../../include/constants/ability.h"
#include "../../include/constants/battle_script_constants.h"
#include "../../include/constants/battle_message_constants.h"
#include "../../include/constants/file.h"
#include "../../include/constants/hold_item_effects.h"
#include "../../include/constants/item.h"
#include "../../include/constants/moves.h"
#include "../../include/constants/move_effects.h"
#include "../../include/constants/species.h"
#include "../../include/constants/system_control.h"

/********************************************************************************************************************/
/********************************************************************************************************************/
//                                                技能使用前判断
/********************************************************************************************************************/
/********************************************************************************************************************/

enum {
    SEQ_MEGA_CHECK = 0,
    SEQ_SENSEI_CHECK,
    SEQ_STATUS_CHECK,
    SEQ_BADGE_CHECK,
    SEQ_PP_CHECK,
    SEQ_DEFENCE_CHECK,
    SEQ_WAZAKOYUU_CHECK,
    SEQ_DEFENCE_CHANGE_CHECK,
    SEQ_PROTEAN_CHECK,
    SEQ_STANCE_CHANGE_CHECK,
    SEQ_PARENTAL_BOND_CHECK,
};

enum ObedienceCheckResult {
    OBEY_CHECK_SUCCESS = 0,
    OBEY_CHECK_DO_NOTHING,
    OBEY_CHECK_DIFFERENT_MOVE,
    OBEY_CHECK_HIT_SELF
};

enum {
    CHECK_STATUS_START = 0,

    CHECK_STATUS_STATE_SLEEP,
    CHECK_STATUS_STATE_FREEZE,
    CHECK_STATUS_STATE_TRUANT,
    CHECK_STATUS_STATE_RECHARGING,
    CHECK_STATUS_STATE_FLINCH,
    CHECK_STATUS_STATE_DISABLE,
    CHECK_STATUS_STATE_TAUNT,
    CHECK_STATUS_STATE_IMPRISON,
    CHECK_STATUS_STATE_GRAVITY,
    CHECK_STATUS_STATE_HEAL_BLOCK,
    CHECK_STATUS_STATE_CONFUSION,
    CHECK_STATUS_STATE_PARALYSIS,
    CHECK_STATUS_STATE_ATTRACT,
    CHECK_STATUS_STATE_BIDE,
    CHECK_STATUS_STATE_SELF_THAW,

    CHECK_STATUS_END,
};

enum {
    CHECK_STATUS_LOOP_BACK = 0,
    CHECK_STATUS_DISRUPT_MOVE,  // wholly disrupt the move; attacker does not get a turn
    CHECK_STATUS_GO_TO_SCRIPT,  // execute a given script, then proceed with the chosen move
    CHECK_STATUS_DONE,
};


// this file's functions
void UNUSED BattleController_BeforeMove(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckRecharge(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckSleepOrFrozen(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckPP(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckTruant(struct BattleSystem *bsys, struct BattleStruct *ctx);
BOOL BattleController_CheckFocusPunch(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckFlinch(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckDisabled(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckHealBlock(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckGravityOrThroatChop(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckTaunt(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckImprison(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckConfusion(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckParalysis(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckInfatuation(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckStanceChange(struct BattleSystem *bsys, struct BattleStruct *ctx);
BOOL BattlerController_RedirectTarget(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckThawOut(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx);
void BattleController_CheckSubmove(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx);
BOOL BattleController_CheckBurnUpOrDoubleShock(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx);
BOOL BattleController_CheckPrimalWeather(struct BattleSystem *bsys, struct BattleStruct *ctx);
BOOL BattleController_CheckMoveFailures1(struct BattleSystem *bsys, struct BattleStruct *ctx);
void BattleController_CheckBide(struct BattleSystem *bsys, struct BattleStruct *ctx);
BOOL BattleController_CheckAbilityFailures1(struct BattleSystem *bsys, struct BattleStruct *ctx);
BOOL BattleController_CheckInterruptibleMoves(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx);
BOOL BattleController_CheckChargeMoves(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx);
BOOL BattleController_CheckStolenBySnatch(struct BattleSystem *bw UNUSED, struct BattleStruct *sp);
BOOL BattleController_CheckSemiInvulnerability(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckProtect(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckPsychicTerrain(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckTelekinesis(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL CalcDamageAndSetMoveStatusFlags(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckTypeImmunity(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckLevitate(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckAirBalloonTelekinesisMagnetRise(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckSafetyGoggles(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckAbilityFailures3(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckTypeBasedMoveConditionImmunities1(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckMoveFailures2(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckMoveFailures2_VenomDrench(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx UNUSED, int defender UNUSED);
BOOL BattleController_CheckMoveFailures3(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckMoveFailures3_StatsChanges(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckMoveFailures3_PerishSong(struct BattleSystem *bsys, struct BattleStruct *ctx);
BOOL BattleController_CheckWhirlwindFailures(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckUproarStoppingSleepMoves(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckSafeguard(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckTerrainBlock(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
int BattlerController_CheckSubstituteBlockingStatDropsOrDecorate(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender);
BOOL BattlerController_CheckMist(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckAbilityFailures4_StatBasedFailures(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckAbilityFailures4_StatusBasedFailures(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckAbilityFailures4_OtherAromaVeilSturdy(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx);
BOOL BattleController_CheckMoveAccuracy(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckSubstituteBlockingOtherEffects(struct BattleSystem *bsys, struct BattleStruct *ctx);
BOOL BattleController_CheckMoveFailures5(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckAromaVeil(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx UNUSED, int defender UNUSED);
BOOL IfAllClientsHavePerishSong(struct BattleSystem *bsys, struct BattleStruct *ctx);
BOOL BattleController_CheckTypeBasedMoveConditionImmunities2(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);
BOOL BattleController_CheckStrongWindsWeaken(struct BattleSystem *bw, struct BattleStruct *sp, int defender);
BOOL BattleController_CheckTeraShell(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx UNUSED, int defender UNUSED);
BOOL BattleController_TryConsumeDamageReductionBerry(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender);

// 08014ACC

/**
 *  @brief run through everything before any of the moves are used
 *         modified with sweat and tears
 * ov12_0224C38C https://github.com/pret/pokeheartgold/blob/fa9d1977b4bf4f7439cb02ebb31c1dafd766d49c/src/battle/battle_controller_player.c#L2646
 * BattleController_BeforeMove https://github.com/pret/pokeplatinum/blob/1422848525cf1ce0cc89348f3b63fec8d086596a/src/battle/battle_controller.c#L3137
 *
 *  @param bsys battle work structure
 *  @param ctx global battle structure
 */
void __attribute__((section (".init"))) BattleController_BeforeMove(struct BattleSystem *bsys, struct BattleStruct *ctx) {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
    debug_printf("In ServerWazaBefore\n");
#endif

    CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);

    if (ctx->waza_status_flag & WAZA_STATUS_FLAG_NO_OUT) {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
        debug_printf("WAZA_STATUS_FLAG_NO_OUT set, check Metronome");
#endif
        ctx->server_seq_no = CONTROLLER_COMMAND_26;
        ST_ServerMetronomeBeforeCheck(bsys, ctx);  // 801ED20h
        return;
    }

    switch (ctx->wb_seq_no) {
        case BEFORE_MOVE_START: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_START\n");
#endif

            ctx->battlemon[ctx->attack_client].condition2 &= ~STATUS2_DESTINY_BOND;
            ctx->battlemon[ctx->attack_client].effect_of_moves &= ~MOVE_EFFECT_FLAG_GRUDGE;
            // reset new stuff here, because subscript is modified
            ctx->battlemon[ctx->attack_client].moveeffect.custapBerryFlag = 0;
            ctx->battlemon[ctx->attack_client].moveeffect.quickClawFlag = 0;
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_RECHARGE: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_RECHARGE\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckRecharge(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_SLEEP_OR_FROZEN: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_SLEEP_OR_FROZEN\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckSleepOrFrozen(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_CHECK_OBEDIENCE: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_CHECK_OBEDIENCE\n");
#endif

            int ret;
            int seq_no;

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_OBEDIENCE_CHECK) == 0) {
                ret = ServerBadgeCheck(bsys, ctx, &seq_no);  // 8013610h
                if (ret) {
                    switch (ret) {
                        case OBEY_CHECK_DO_NOTHING:
                            ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
                            break;
                        case OBEY_CHECK_DIFFERENT_MOVE:
                            ctx->next_server_seq_no = ctx->server_seq_no;
                            break;
                        case OBEY_CHECK_HIT_SELF:
                            ctx->next_server_seq_no = CONTROLLER_COMMAND_34;
                            break;
                    }
                    ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                    LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, seq_no);
                    return;
                }
            }

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_CHECK_PP: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_CHECK_PP\n");
#endif

            BattleController_CheckPP(bsys, ctx);
            ctx->wb_seq_no++;
            return;
        }
        // TODO implement new mechanics
        case BEFORE_MOVE_STATE_DISPLAY_Z_DANCE_AND_EFFECT: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_DIctxLAY_Z_DANCE_AND_EFFECT\n");
#endif

            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_TRUANT: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_TRUANT\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckTruant(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_FOCUS_PUNCH_LOSE_FOCUS: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_FOCUS_PUNCH_LOSE_FOCUS\n");
#endif

            ctx->wb_seq_no++;
            if (BattleController_CheckFocusPunch(bsys, ctx)) {
                return;
            }
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_FLINCH: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_FLINCH\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckFlinch(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_DISABLED: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_DISABLED\n");
#endif
            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckDisabled(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_HEAL_BLOCK: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_HEAL_BLOCK\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckHealBlock(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_GRAVITY_THROAT_CHOP: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_GRAVITY_THROAT_CHOP\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckGravityOrThroatChop(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        // TODO: implement new mechanics (Magic Room/Dance before coming back to this)
        case BEFORE_MOVE_STATE_CHECK_CHOICE_LOCK: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_CHECK_CHOICE_LOCK\n");
#endif

            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_TAUNT: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_TAUNT\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckTaunt(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_IMPRISION: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_IMPRISION\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckImprison(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_CONFUSION_SELF_HIT_OR_WEAR_OFF: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_CONFUSION_SELF_HIT_OR_WEAR_OFF\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckConfusion(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_PARALYSIS: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_PARALYSIS\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckParalysis(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_INFATUATION: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_INFATUATION\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckInfatuation(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_ANNOUNCE_SUB_MOVE: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_ANNOUNCE_SUB_MOVE\n");
#endif

#ifdef DEBUG_BEFORE_MOVE_LOGIC
            // debug_printf("current_move_index: %d\n", ctx->current_move_index);
            // debug_printf("moveNoTemp: %d\n", ctx->moveNoTemp);
#endif

            BattleController_CheckSubmove(bsys, ctx);
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_THAW_OUT_BY_MOVE: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_THAW_OUT_BY_MOVE\n");
#endif

            BattleController_CheckThawOut(bsys, ctx);
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_STANCE_CHANGE: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_STANCE_CHANGE\n");
#endif

            BattleController_CheckStanceChange(bsys, ctx);
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_MOVE_TYPE_CHANGES: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MOVE_TYPE_CHANGES\n");
#endif

            ctx->move_type = GetAdjustedMoveType(ctx, ctx->attack_client, ctx->current_move_index);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_REDIRECT_TARGET: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_REDIRECT_TARGET\n");
#endif

            ctx->wb_seq_no++;
            if (BattlerController_RedirectTarget(bsys, ctx) == TRUE) {
                return;
            }
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_DECREMENT_PP: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_DECREMENT_PP\n");
#endif

#ifdef DEBUG_BEFORE_MOVE_LOGIC
            // debug_printf("current_move_index: %d\n", ctx->current_move_index);
            // debug_printf("moveNoTemp: %d\n", ctx->moveNoTemp);
#endif

            if ((ctx->waza_out_check_on_off & 0x8) == 0) {
                // pp检查
                if (ServerPPCheck(bsys, ctx) == TRUE)  // 801393Ch
                {
                    return;
                }
            }
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO: check correctness
        case BEFORE_MOVE_STATE_CHOICE_LOCK: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_CHOICE_LOCK\n");
#endif

            debug_printf("current_move_index: %d\n", ctx->current_move_index);
            int itemEffect = GET_HELD_ITEM_HOLD_EFFECT_ACCOUNTING_KLUTZ(ctx, ctx->attack_client);
            if (itemEffect == HOLD_EFFECT_CHOICE_ATK || itemEffect == HOLD_EFFECT_CHOICE_SPEED || itemEffect == HOLD_EFFECT_CHOICE_SPATK) {
                if (ctx->waza_work != MOVE_STRUGGLE
                && (ctx->waza_work != MOVE_U_TURN || (ctx->server_status_flag2 & SYSCTL_UTURN_ACTIVE) == FALSE)
                && (ctx->waza_work != MOVE_BATON_PASS || (ctx->server_status_flag2 & SYSCTL_MOVE_SUCCEEDED) == FALSE)) {
                    ctx->battlemon[ctx->attack_client].moveeffect.moveNoChoice = ctx->current_move_index;
                } else {
                    ctx->battlemon[ctx->attack_client].moveeffect.moveNoChoice = MOVE_NONE;
                }
            }
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_BURN_UP_OR_DOUBLE_SHOCK: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_BURN_UP_OR_DOUBLE_SHOCK\n");
#endif

            ctx->wb_seq_no++;
            if (BattleController_CheckBurnUpOrDoubleShock(bsys, ctx)) {
                return;
            }
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_PRIMAL_WEATHER: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_PRIMAL_WEATHER\n");
#endif

            ctx->wb_seq_no++;
            if (BattleController_CheckPrimalWeather(bsys, ctx)) {
                return;
            }
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_CONSUME_MICLE_BERRY_FLAG: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_CONSUME_MICLE_BERRY_FLAG\n");
#endif

            if (ctx->battlemon[ctx->attack_client].moveeffect.boostedAccuracy) {
                ctx->boostedAccuracy = TRUE;
                ctx->battlemon[ctx->attack_client].moveeffect.boostedAccuracy = 0;
            }

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_MOVE_FAILURES_1: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MOVE_FAILURES_1\n");
#endif

            ctx->wb_seq_no++;
            if (BattleController_CheckMoveFailures1(bsys, ctx)) {
                return;
            }
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_BIDE: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_BIDE\n");
#endif

            if ((ctx->waza_out_check_on_off & SYSCTL_SKIP_STATUS_CHECK) == FALSE) {
                BattleController_CheckBide(bsys, ctx);
            }
            ctx->wb_seq_no++;
            return;
        }
        case BEFORE_MOVE_STATE_ABILITY_FAILURES_1: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_ABILITY_FAILURES_1\n");
#endif
            ctx->wb_seq_no++;
            if (BattleController_CheckAbilityFailures1(bsys, ctx)) {
                return;
            }
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_INTERRUPTIBLE_MOVES: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_INTERRUPTIBLE_MOVES\n");
#endif

            ctx->wb_seq_no++;
            if (BattleController_CheckInterruptibleMoves(bsys, ctx)) {
                return;
            }
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_PROTEAN_OR_LIBERO: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_PROTEAN_OR_LIBERO\n");
#endif

            if ((ctx->battlemon[ctx->attack_client].ability == ABILITY_PROTEAN || ctx->battlemon[ctx->attack_client].ability == ABILITY_LIBERO)
                // if either type is not the move's type
                && (ctx->battlemon[ctx->attack_client].type1 != ctx->moveTbl[ctx->current_move_index].type || ctx->battlemon[ctx->attack_client].type2 != ctx->moveTbl[ctx->current_move_index].type)
                // Protean should activate only once per switch-in if gen 9 behavior
                && (ctx->battlemon[ctx->attack_client].ability_activated_flag == 0 || PROTEAN_GENERATION < 9)
                // the move has to have power in order for it to change the type
                && ctx->moveTbl[ctx->current_move_index].power != 0) {
                ctx->battlemon[ctx->attack_client].type1 = ctx->moveTbl[ctx->current_move_index].type;
                ctx->battlemon[ctx->attack_client].type2 = ctx->moveTbl[ctx->current_move_index].type;
#if PROTEAN_GENERATION >= 9
                ctx->battlemon[ctx->attack_client].ability_activated_flag = 1;
#endif
                LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_HANDLE_PROTEAN_MESSAGE);
                ctx->msg_work = ctx->battlemon[ctx->attack_client].type1;
                ctx->battlerIdTemp = ctx->attack_client;
                ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
                ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                return;
            } else {
                ctx->wb_seq_no++;
            }
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_CHARGING_MOVE_MESSAGE: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_CHARGING_MOVE_MESSAGE\n");
#endif

            ctx->wb_seq_no++;
            if (BattleController_CheckChargeMoves(bsys, ctx)) {
                return;
            }
            FALLTHROUGH;
        }
        // TODO: split ServerWazaKoyuuCheck function, modernise
        case BEFORE_MOVE_STATE_CHECK_STOLEN_BY_SNATCH: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_CHECK_STOLEN_BY_SNATCH\n");
#endif

            if ((ctx->waza_out_check_on_off & 0x80) == 0) {
                if (ServerWazaKoyuuCheck(bsys, ctx) == TRUE && !ctx->moveStolen)  // 8014944h
                {
                    return;
                }
            }
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO: come back during simultaneous damage
        case BEFORE_MOVE_STATE_SET_EXPLOSION_SELF_DESTRUCT_FLAG: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_SET_EXPLOSION_SELF_DESTRUCT_FLAG\n");
#endif

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // 攻击对象检查，包括了蓄力技能
        case BEFORE_MOVE_STATE_CHECK_NO_TARGET_OR_SELF: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_CHECK_NO_TARGET_OR_SELF\n");
#endif

            if (ServerDefenceCheck(bsys, ctx) == TRUE)  // 8013AD8h
            {
                return;
            }
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO implement new mechanics
        case BEFORE_MOVE_STATE_SET_STEEL_BEAM_FLAG: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_SET_STEEL_BEAM_FLAG\n");
#endif

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO implement new mechanics
        case BEFORE_MOVE_STATE_CHECK_SKY_DROP_TARGET: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_CHECK_SKY_DROP_TARGET\n");
#endif

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_SEMI_INVULNERABILITY: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_SEMI_INVULNERABILITY\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckSemiInvulnerability);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_PSYCHIC_TERRAIN: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_PSYCHIC_TERRAIN\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckPsychicTerrain);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO implement new mechanics
        case BEFORE_MOVE_STATE_TEAMMATE_PROTECTION: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_TEAMMATE_PROTECTION\n");
#endif

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_PROTECT_AND_FRIENDS: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_PROTECT_AND_FRIENDS\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckProtect);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO implement new mechanics
        case BEFORE_MOVE_STATE_MAT_BLOCK: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MAT_BLOCK\n");
#endif

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO implement new mechanics
        case BEFORE_MOVE_STATE_MAX_GUARD: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MAX_GUARD\n");
#endif

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO: split ServerWazaKoyuuCheck function, modernise
        case BEFORE_MOVE_STATE_MAGIC_COAT: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MAGIC_COAT\n");
#endif

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_TELEKINESIS_FAILURES: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_TELEKINESIS_FAILURES\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckTelekinesis);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO: split ServerWazaKoyuuCheck function, modernise
        case BEFORE_MOVE_STATE_MAGIC_BOUNCE: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MAGIC_BOUNCE\n");
#endif

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO: check correctness, handle Wonder Guard here, rewrite it ourselves
        case BEFORE_MOVE_STATE_ABILITY_FAILURES_2: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_ABILITY_FAILURES_2\n");
#endif
            if (!(ctx->waza_out_check_on_off & 0x10) && ctx->defence_client != 0xFF && ov12_0224BC2C(bsys, ctx) == TRUE) {
                return;
            }
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO: check correctness, rewrite it ourselves
        case BEFORE_MOVE_STATE_TYPE_CHART_IMMUNITY: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_TYPE_CHART_IMMUNITY\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckTypeImmunity);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_LEVITATE: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_LEVITATE\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckLevitate);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_AIR_BALLOON_TELEKINESIS_MAGNET_RISE: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_AIR_BALLOON_TELEKINESIS_MAGNET_RISE\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckAirBalloonTelekinesisMagnetRise);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_SAFETY_GOGGLES: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_SAFETY_GOGGLES\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckSafetyGoggles);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_ABILITY_FAILURES_3: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_ABILITY_FAILURES_3\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckAbilityFailures3);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_TYPE_BASED_MOVE_CONDITION_IMMUNITIES_1: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_TYPE_BASED_MOVE_CONDITION_IMMUNITIES_1\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckTypeBasedMoveConditionImmunities1);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_MOVE_FAILURES_2: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MOVE_FAILURES_2\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckMoveFailures2);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_MOVE_FAILURES_2_VENOM_DRENCH: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MOVE_FAILURES_2_VENOM_DRENCH\n");
#endif
            LoopCheckFunctionForSpreadMove_StatFailureSuccessCheck(bsys, ctx, BattleController_CheckMoveFailures2_VenomDrench);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_MOVE_FAILURES_3: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MOVE_FAILURES_3\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckMoveFailures3);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_MOVE_FAILURES_3_PERISH_SONG: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MOVE_FAILURES_3_PERISH_SONG\n");
#endif

            ctx->wb_seq_no++;
            // If another failure condition would prevent success (e.g. semi-invulernability), both failure messages play.
            if (BattleController_CheckMoveFailures3_PerishSong(bsys, ctx)) {
                return;
            }
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_MOVE_FAILURES_3_LOWER_STATS: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MOVE_FAILURES_3_LOWER_STATS\n");
#endif

            LoopCheckFunctionForSpreadMove_StatFailureSuccessCheck_StatChanges(bsys, ctx, BattleController_CheckMoveFailures3_StatsChanges);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_TYPE_BASED_MOVE_CONDITION_IMMUNITIES_2: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_TYPE_BASED_MOVE_CONDITION_IMMUNITIES_2\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckTypeBasedMoveConditionImmunities2);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_UPROAR_STOPPING_MOVES: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_UPROAR_STOPPING_MOVES\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckUproarStoppingSleepMoves);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_SAFEGUARD: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_SAFEGUARD\n");
#endif
            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckSafeguard);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_TERRAIN_BLOCK: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_TERRAIN_BLOCK\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckTerrainBlock);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_SUBSTITUTE_BLOCKING_STAT_DROPS_DECORATE: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_SUBSTITUTE_BLOCKING_STAT_DROPS_DECORATE\n");
#endif
            LoopCheckFunctionForSpreadMove_StatFailureSuccessCheck(bsys, ctx, BattlerController_CheckSubstituteBlockingStatDropsOrDecorate)
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_MIST: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MIST\n");
#endif

            LoopCheckFunctionForSpreadMove_StatFailureSuccessCheck(bsys, ctx, BattlerController_CheckMist);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_ABILITY_FAILURES_4_STAT_BASED_FAILURES: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_ABILITY_FAILURES_4_STAT_BASED_FAILURES\n");
#endif

            LoopCheckFunctionForSpreadMove_StatFailureSuccessCheck_StatChanges(bsys, ctx, BattleController_CheckAbilityFailures4_StatBasedFailures);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_ABILITY_FAILURES_4_STATUS_BASED_FAILURES: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_ABILITY_FAILURES_4_STATUS_BASED_FAILURES\n");
#endif

            LoopCheckFunctionForSpreadMove_RawSpeedWithNonRNGTie(bsys, ctx, BattleController_CheckAbilityFailures4_StatusBasedFailures);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_ABILITY_FAILURES_4_OTHER_AROMA_VEIL_STRUDY: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_ABILITY_FAILURES_4_OTHER_AROMA_VEIL_STRUDY\n");
#endif

            ctx->wb_seq_no++;
            BattleController_CheckAbilityFailures4_OtherAromaVeilSturdy(bsys, ctx);
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_MOVE_ACCURACY: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MOVE_ACCURACY\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckMoveAccuracy);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_SUBSTITUTE_BLOCKING_OTHER_EFFECTS: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_SUBSTITUTE_BLOCKING_OTHER_EFFECTS\n");
#endif

            ctx->wb_seq_no++;
            BattleController_CheckSubstituteBlockingOtherEffects(bsys, ctx);
            FALLTHROUGH;
        }
        // TODO implement new mechanics
        case BEFORE_MOVE_STATE_MIRROR_ARMOR: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MIRROR_ARMOR\n");
#endif

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_ROAR_WHIRLWIND_INTO_DYNAMAXED_TARGET: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_ROAR_WHIRLWIND_INTO_DYNAMAXED_TARGET\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckWhirlwindFailures);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        // TODO
        case BEFORE_MOVE_STATE_MOVE_FAILURES_4: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MOVE_FAILURES_4\n");
#endif

            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_MOVE_FAILURES_5: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_MOVE_FAILURES_5\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckMoveFailures5);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_AROMA_VEIL: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_AROMA_VEIL\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckAromaVeil);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_TRIGGER_STRONG_WINDS: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_TRIGGER_STRONG_WINDS\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckStrongWindsWeaken);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_STATE_TERA_SHELL: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_TERA_SHELL\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_CheckTeraShell);
            ctx->wb_seq_no++;
            FALLTHROUGH;            
        }
        case BEFORE_MOVE_STATE_CONSUME_DAMAGE_REDUCING_BERRY: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_STATE_CONSUME_DAMAGE_REDUCING_BERRY\n");
#endif

            LoopCheckFunctionForSpreadMove(bsys, ctx, BattleController_TryConsumeDamageReductionBerry);
            ctx->wb_seq_no++;
            FALLTHROUGH;
        }
        case BEFORE_MOVE_END: {
#ifdef DEBUG_BEFORE_MOVE_LOGIC
            debug_printf("In BEFORE_MOVE_END\n");
#endif

            // TODO: move it to the end of TryMove after the entire overhaul, but this works perfectly fine here
            // Edit: Should be fine after the function merges by Blu
            if (IsValidParentalBondMove(bsys, ctx, FALSE) &&
                ctx->loop_hit_check != 0xFD) {
                ctx->multi_hit_count = 2;
                ctx->multi_hit_count_temp = 2;
                ctx->loop_hit_check = 0xFD;
                ctx->oneTurnFlag[ctx->battlerIdTemp].parental_bond_is_active = TRUE;
            } else {
                ctx->oneTurnFlag[ctx->battlerIdTemp].parental_bond_is_active = FALSE;
                ctx->wb_seq_no = 0;
            }

            ctx->wb_seq_no = BEFORE_MOVE_START;
            break;
        }
    }

    // Redirect to original TryMove
    ctx->server_seq_no = CONTROLLER_COMMAND_24;
    if (ctx->waza_status_flag & WAZA_STATUS_FLAG_NO_OUT) {
        ctx->server_seq_no = CONTROLLER_COMMAND_26;
    } else {
        // ctx->server_status_flag2 |= BATTLE_STATUS2_MOVE_SUCCEEDED;
        // ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;  // execute the move
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_MOVE_SEQ, ctx->current_move_index);
        ctx->server_seq_no = CONTROLLER_COMMAND_24;
        // ctx->next_server_seq_no = CONTROLLER_COMMAND_24;  // after that
        ST_ServerTotteokiCountCalc(bsys, ctx);              // 801B570h
    }
    ST_ServerMetronomeBeforeCheck(bsys, ctx);  // 801ED20h

    return;
}

void BattleController_CheckRecharge(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (ctx->battlemon[ctx->attack_client].condition2 & STATUS2_RECHARGE) {
        ctx->battlemon[ctx->attack_client].condition2 &= ~STATUS2_RECHARGE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_RECHARGE_TURN);
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
        ctx->wb_seq_no = BEFORE_MOVE_START;
        CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
        ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
    }
}

void BattleController_CheckSleepOrFrozen(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    int effect = ctx->moveTbl[ctx->current_move_index].effect;

    if (ctx->battlemon[ctx->attack_client].condition & STATUS_SLEEP) {
        if (ctx->field_condition & FIELD_STATUS_UPROAR && GetBattlerAbility(ctx, ctx->attack_client) != ABILITY_SOUNDPROOF) {
            ctx->battlerIdTemp = ctx->attack_client;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_WAKE_UP);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            return;
        } else if ((ctx->current_move_index != MOVE_SLEEP_TALK && ctx->moveNoTemp == MOVE_SLEEP_TALK) == 0) {
            u32 sleepCounterDecrease;

            if (GetBattlerAbility(ctx, ctx->attack_client) == ABILITY_EARLY_BIRD) {
                sleepCounterDecrease = 2;
            } else {
                sleepCounterDecrease = 1;
            }
            if ((ctx->battlemon[ctx->attack_client].condition & STATUS_SLEEP) < sleepCounterDecrease) {
                ctx->battlemon[ctx->attack_client].condition &= ~STATUS_SLEEP;
            } else {
                ctx->battlemon[ctx->attack_client].condition -= sleepCounterDecrease;
            }

            if (ctx->battlemon[ctx->attack_client].condition & STATUS_SLEEP) {
                if (ctx->current_move_index != MOVE_SNORE && ctx->moveNoTemp != MOVE_SLEEP_TALK) {
                    LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_SLEEPING);
                    ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                    ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
                    ctx->wb_seq_no = BEFORE_MOVE_START;
                    CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
                    ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
                    ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
                    return;
                }
            } else {
                ctx->battlerIdTemp = ctx->attack_client;
                LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_WAKE_UP);
                ctx->next_server_seq_no = ctx->server_seq_no;
                ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                return;
            }
        }
    }

    if (ctx->battlemon[ctx->attack_client].condition & STATUS_FREEZE) {
        if (BattleRand(bsys) % 5 != 0) {
            if (effect != MOVE_EFFECT_THAW_AND_BURN_HIT && effect != MOVE_EFFECT_RECOIL_BURN_HIT) {
                LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_FROZEN);
                ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
                ctx->wb_seq_no = BEFORE_MOVE_START;
                CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
                ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
                ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
                return;
            }
        } else {
            ctx->battlerIdTemp = ctx->attack_client;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_THAW_OUT);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            return;
        }
    }
}

void BattleController_CheckPP(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    int index = BattleMon_GetMoveIndex(&ctx->battlemon[ctx->attack_client], ctx->moveNoTemp);

    if (ctx->battlemon[ctx->attack_client].pp[index] == 0) {
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_NO_PP_LEFT);
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
        ctx->wb_seq_no = BEFORE_MOVE_START;
        CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
        ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
    }
}

void BattleController_CheckTruant(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (CheckTruant(ctx, ctx->attack_client) == TRUE) {
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_LOAFING);
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
        ctx->wb_seq_no = BEFORE_MOVE_START;
        CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
        ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
    }
}

BOOL BattleController_CheckFocusPunch(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx) {

    if (
        ((ctx->oneTurnFlag[ctx->attack_client].physical_damager_bit) || (ctx->oneTurnFlag[ctx->attack_client].special_damager_bit))
        && (ctx->oneTurnFlag[ctx->attack_client].pendingFocusPunchFlag)
        && (ctx->current_move_index == MOVE_FOCUS_PUNCH)) {
            //https://www.smogon.com/forums/threads/sword-shield-battle-mechanics-research.3655528/page-60#post-8755569
            // The "Pokemon will flinch" flag and the "Focus Punch will lose focus" flag are dichotomous.
            // idk what dichotomous means, but this seems to be aligned with the video evidence
            ctx->battlemon[ctx->attack_client].condition2 &= ~STATUS2_FLINCH;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_LOST_FOCUS);
            ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
            return TRUE;
    }
    return FALSE;
}

void BattleController_CheckFlinch(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (ctx->battlemon[ctx->attack_client].condition2 & STATUS2_FLINCH) {
        ctx->battlemon[ctx->attack_client].condition2 &= ~STATUS2_FLINCH;
        ctx->moveOutCheck[ctx->attack_client].stoppedFromFlinch = TRUE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_MOVE_FAIL_FLINCHED);
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
        ctx->wb_seq_no = BEFORE_MOVE_START;
        CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
        ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
    }
}

void BattleController_CheckDisabled(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (ctx->battlemon[ctx->attack_client].moveeffect.disabledMove == ctx->moveNoTemp) {
        ctx->moveOutCheck[ctx->attack_client].stoppedFromDisable = TRUE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_MOVE_IS_DISABLED);
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
        ctx->wb_seq_no = BEFORE_MOVE_START;
        CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
        ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
    }
}

// TODO: Modernise Heal Block mechanics
void BattleController_CheckHealBlock(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (BattleContext_CheckMoveHealBlocked(bsys, ctx, ctx->attack_client, ctx->current_move_index)) {
        ctx->moveOutCheck[ctx->attack_client].stoppedFromHealBlock = TRUE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_MOVE_FAILED_HEAL_BLOCK);
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
        ctx->wb_seq_no = BEFORE_MOVE_START;
        CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
        ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
    }
}

// TODO: Gravity ban list and Throat Chop
void BattleController_CheckGravityOrThroatChop(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (BattleContext_CheckMoveUnuseableInGravity(bsys, ctx, ctx->attack_client, ctx->current_move_index)) {
        ctx->moveOutCheck[ctx->attack_client].stoppedFromGravity = TRUE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_MOVE_FAIL_GRAVITY);
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
        ctx->wb_seq_no = BEFORE_MOVE_START;
        CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
        ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
    }
}

void BattleController_CheckTaunt(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (ctx->battlemon[ctx->attack_client].moveeffect.tauntTurns && ctx->moveTbl[ctx->current_move_index].power == 0) {
        ctx->moveOutCheck[ctx->attack_client].stoppedFromTaunt = TRUE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_MOVE_FAIL_TAUNTED);
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
        ctx->wb_seq_no = BEFORE_MOVE_START;
        CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
        ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
    }
}

void BattleController_CheckImprison(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (BattleContext_CheckMoveImprisoned(bsys, ctx, ctx->attack_client, ctx->current_move_index)) {
        ctx->moveOutCheck[ctx->attack_client].stoppedFromImprison = TRUE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_MOVE_IS_IMPRISONED);
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
        ctx->wb_seq_no = BEFORE_MOVE_START;
        CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
        ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
    }
}

void BattleController_CheckConfusion(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (ctx->battlemon[ctx->attack_client].condition2 & STATUS2_CONFUSION) {
        ctx->battlemon[ctx->attack_client].condition2 -= 1;
        if (ctx->battlemon[ctx->attack_client].condition2 & STATUS2_CONFUSION) {
            // modernised to 33%
            if (BattleRand(bsys) % 3 != 0) {
                LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_CONFUSED);
                ctx->next_server_seq_no = ctx->server_seq_no;
                ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            } else {
                LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_CONFUSED_SELF_HIT);
                ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                ctx->next_server_seq_no = CONTROLLER_COMMAND_34;
                ctx->wb_seq_no = BEFORE_MOVE_START;
                CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
                ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
                ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
            }
        } else {
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_CONFUSED_NO_MORE);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        }
    }
}

void BattleController_CheckParalysis(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (ctx->battlemon[ctx->attack_client].condition & STATUS_PARALYSIS) {
        if (BattleRand(bsys) % 4 == 0) {
            ctx->moveOutCheck[ctx->attack_client].stoppedFromParalysis = TRUE;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_FULLY_PARALYZED);
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
            ctx->wb_seq_no = BEFORE_MOVE_START;
            CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
            ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
            ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
        }
    }
}

void BattleController_CheckInfatuation(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (ctx->battlemon[ctx->attack_client].condition2 & STATUS2_ATTRACT) {
        ctx->battlerIdTemp = LowestFlagNo((ctx->battlemon[ctx->attack_client].condition2 & STATUS2_ATTRACT) >> STATUS2_ATTRACT_SHIFT);
        if (BattleRand(bsys) & 1) {
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_INFATUATED);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        } else {
            ctx->moveOutCheck[ctx->attack_client].stoppedFromAttract = TRUE;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_IMMOBILIZED_BY_LOVE);
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
            ctx->wb_seq_no = BEFORE_MOVE_START;
            CopyBattleMonToPartyMon(bsys, ctx, ctx->attack_client);
            ctx->server_status_flag |= BATTLE_STATUS_CHECK_LOOP_ONLY_ONCE;
            ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
        }
    }
}

// void BattleController_CheckSleepAnnouncement(struct BattleSystem *bsys, struct BattleStruct *ctx) {
//     if (ctx->battlemon[ctx->attack_client].condition & STATUS_SLEEP) {
//         if (ctx->current_move_index == MOVE_SNORE || ctx->moveNoTemp == MOVE_SLEEP_TALK) {
//             LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_SLEEPING);
//             ctx->next_server_seq_no = ctx->server_seq_no;
//             ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
//             return;
//         }
//     }
// }

// TODO: unique subscript for Stance Change
void BattleController_CheckStanceChange(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (ctx->battlemon[ctx->attack_client].ability == ABILITY_STANCE_CHANGE && ctx->battlemon[ctx->attack_client].species == SPECIES_AEGISLASH) {
        ctx->battlerIdTemp = ctx->attack_client;
        if (ctx->current_move_index == MOVE_KINGS_SHIELD && ctx->battlemon[ctx->attack_client].form_no == 1) {
            ctx->battlemon[ctx->battlerIdTemp].form_no = 0;
            BattleFormChange(ctx->battlerIdTemp, ctx->battlemon[ctx->battlerIdTemp].form_no, bsys, ctx, 0);
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_FORM_CHANGE);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        } else if (ctx->moveTbl[ctx->current_move_index].power != 0 && ctx->battlemon[ctx->attack_client].form_no == 0) {
            ctx->battlemon[ctx->battlerIdTemp].form_no = 1;
            BattleFormChange(ctx->battlerIdTemp, ctx->battlemon[ctx->battlerIdTemp].form_no, bsys, ctx, 0);
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_FORM_CHANGE);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        }
    }
}

// TODO: Surely this can be simplified? When Mirror Coat is also modernised?
BOOL BattlerController_RedirectTarget(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    int side;
    int battlerId;
    int battlerIdTarget;
    int moveType;
    int maxBattlers;
    int battlerIdAttacker = ctx->attack_client;
    int range;

    if (ctx->defence_client == BATTLER_NONE) {
        return FALSE;
    }

    if (GetBattlerAbility(ctx, battlerIdAttacker) == ABILITY_NORMALIZE || GetBattlerAbility(ctx, battlerIdAttacker) == ABILITY_MOLD_BREAKER) {
        return FALSE;
    }

    side = IsClientEnemy(bsys, battlerIdAttacker) ^ 1;

    if (ctx->scw[side].followMeFlag && ctx->battlemon[ctx->scw[side].battlerIdFollowMe].hp) {
        return FALSE;
    }

    moveType = ctx->move_type;
    if (!moveType) {
        moveType = ctx->moveTbl[ctx->current_move_index].type;
    }

    range = ctx->moveTbl[ctx->current_move_index].target;

    maxBattlers = BattleWorkClientSetMaxGet(bsys);

    battlerIdAttacker = ctx->attack_client;
    battlerIdTarget = ctx->defence_client;

    SortRawSpeedNonRNGArray(bsys, ctx);

    if (moveType == TYPE_ELECTRIC && (range == MOVE_TARGET_SELECTED || range == MOVE_TARGET_RANDOM) && !(ctx->server_status_flag & BATTLE_STATUS_CHARGE_TURN) && CheckSideAbility(bsys, ctx, CHECK_ABILITY_ALL_HP_NOT_USER, battlerIdAttacker, ABILITY_LIGHTNING_ROD)) {
        for (battlerId = 0; battlerId < maxBattlers; battlerId++) {
            battlerIdTarget = ctx->rawSpeedNonRNGClientOrder[battlerId];
            if (GetBattlerAbility(ctx, battlerIdTarget) == ABILITY_LIGHTNING_ROD && ctx->battlemon[battlerIdTarget].hp && battlerIdAttacker != battlerIdTarget) {
                break;
            }
        }
        if (battlerIdTarget != ctx->defence_client) {
            ctx->oneSelfFlag[battlerIdTarget].lightningRodFlag = TRUE;
            ctx->defence_client = battlerIdTarget;
        }
    } else if (moveType == TYPE_WATER && (range == MOVE_TARGET_SELECTED || range == MOVE_TARGET_RANDOM) && !(ctx->server_status_flag & BATTLE_STATUS_CHARGE_TURN) && CheckSideAbility(bsys, ctx, CHECK_ABILITY_ALL_HP_NOT_USER, battlerIdAttacker, ABILITY_STORM_DRAIN)) {
        for (battlerId = 0; battlerId < maxBattlers; battlerId++) {
            battlerIdTarget = ctx->rawSpeedNonRNGClientOrder[battlerId];
            if (GetBattlerAbility(ctx, battlerIdTarget) == ABILITY_STORM_DRAIN && ctx->battlemon[battlerIdTarget].hp && battlerIdAttacker != battlerIdTarget) {
                break;
            }
        }
        if (battlerIdTarget != ctx->defence_client) {
            ctx->oneSelfFlag[battlerIdTarget].stormDrainFlag = TRUE;
            ctx->defence_client = battlerIdTarget;
        }
    }


    BOOL ret = FALSE;

    if (!(ctx->waza_status_flag & WAZA_STATUS_FLAG_NO_OUT) && ctx->oneSelfFlag[ctx->defence_client].lightningRodFlag) {
        ctx->oneSelfFlag[ctx->defence_client].lightningRodFlag = FALSE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_LIGHTNING_ROD);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ret = TRUE;
    }

    if (!(ctx->waza_status_flag & WAZA_STATUS_FLAG_NO_OUT) && ctx->oneSelfFlag[ctx->defence_client].stormDrainFlag) {
        ctx->oneSelfFlag[ctx->defence_client].stormDrainFlag = FALSE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_LIGHTNING_ROD);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ret = TRUE;
    }

    return ret;
}

// TODO: handle Burn Up edge case
void BattleController_CheckThawOut(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx) {
    int effect = ctx->moveTbl[ctx->current_move_index].effect;

    if (ctx->battlemon[ctx->attack_client].condition & STATUS_FREEZE) {
        if (effect == MOVE_EFFECT_THAW_AND_BURN_HIT || effect == MOVE_EFFECT_RECOIL_BURN_HIT) {
            ctx->battlerIdTemp = ctx->attack_client;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_THAW_OUT);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            return;
        }
    }
}

// TODO: make it so that it doesn't do redundant damage calculations
void BattleController_CheckSubmove(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx) {
    // Include Me First here for now
    if (ctx->current_move_index == MOVE_COPYCAT ||
        ctx->current_move_index == MOVE_METRONOME ||
        ctx->current_move_index == MOVE_NATURE_POWER ||
        ctx->current_move_index == MOVE_SLEEP_TALK ||
        ctx->current_move_index == MOVE_ME_FIRST) {
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_MOVE_SEQ, ctx->current_move_index);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        // Submove announcement guarantees PP deduction.
        ServerPPCheck(bsys, ctx);
        // Technically the cases after Heal Block should not be checked again but functionally it should not matter?
        ctx->wb_seq_no = BEFORE_MOVE_STATE_HEAL_BLOCK - 1;
        return;
    }
}

BOOL BattleController_CheckPrimalWeather(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    // Handle Extremely Harsh Sunlight and Heavy Rain
    if (!CheckSideAbility(bsys, ctx, CHECK_ABILITY_ALL_HP, 0, ABILITY_CLOUD_NINE) && !CheckSideAbility(bsys, ctx, CHECK_ABILITY_ALL_HP, 0, ABILITY_AIR_LOCK)) {
        if ((ctx->field_condition & WEATHER_EXTREMELY_HARSH_SUNLIGHT) && (ctx->move_type == TYPE_WATER)) {
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_CANCEL_WATER_MOVE);
            ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
            return TRUE;
        }

        if ((ctx->field_condition & WEATHER_HEAVY_RAIN) && (ctx->move_type == TYPE_FIRE)) {
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_CANCEL_FIRE_MOVE);
            ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL BattleController_CheckBurnUpOrDoubleShock(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx) {
    int typeToCheck = 0;
    switch (ctx->current_move_index) {
        case MOVE_BURN_UP:
            typeToCheck = TYPE_FIRE;
            break;
        case MOVE_DOUBLE_SHOCK:
            typeToCheck = TYPE_ELECTRIC;
            break;

        default:
            return FALSE;
            break;
    }

    if (!HasType(ctx, ctx->attack_client, typeToCheck)) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_BUT_IT_FAILED_SPREAD);
        ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleController_CheckMoveFailures1(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    int currentMoveIndex = ctx->current_move_index;
    int moveEffect = ctx->moveTbl[currentMoveIndex].effect;
    struct BattlePokemon attackClient = ctx->battlemon[ctx->attack_client];
    struct BattlePokemon defenceClient = ctx->battlemon[ctx->defence_client];
    int cnt = GetBattlerLearnedMoveCount(bsys, ctx, ctx->attack_client);
    int move;
    int attackerAbility = GetBattlerAbility(ctx, ctx->attack_client);

    // For Sucker Punch
    if (ctx->battlemon[ctx->defence_client].moveeffect.encoredMove && ctx->battlemon[ctx->defence_client].moveeffect.encoredMove == ctx->battlemon[ctx->defence_client].move[ctx->battlemon[ctx->defence_client].moveeffect.encoredMoveIndex]) {
        move = ctx->battlemon[ctx->defence_client].moveeffect.encoredMove;
    } else {
        move = GetBattlerSelectedMove(ctx, ctx->defence_client);
    }

    // TODO: client Transformed into the correct species can use the move as well
    // Dark Void when user isn't Darkrai
    if ((currentMoveIndex == MOVE_DARK_VOID && attackClient.species != SPECIES_DARKRAI)
    // Hyperspace Fury when user isn't Hoopa
    || (currentMoveIndex == MOVE_HYPERSPACE_FURY && attackClient.species != SPECIES_HOOPA)
    // Aura Wheel when user isn't Morpeko
    || (currentMoveIndex == MOVE_AURA_WHEEL && attackClient.species != SPECIES_MORPEKO)) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_CANT_USE_MOVE);
        ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
        return TRUE;
    }
    // Hyperspace Fury when user is Hoopa Confined
    if (currentMoveIndex == MOVE_HYPERSPACE_FURY
    && attackClient.species == SPECIES_HOOPA
    && attackClient.form_no == 0) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_CANT_USE_MOVE_HOOPA_CONFINED);
        ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
        return TRUE;
    }

    // Aurora Veil when it is not hailing
    if ((currentMoveIndex == MOVE_AURORA_VEIL && !(ctx->field_condition & WEATHER_HAIL_ANY || ctx->field_condition & WEATHER_SNOW_ANY))
    // Clangorous Soul when user lacks HP to execute the move
    || ((currentMoveIndex == MOVE_CLANGOROUS_SOUL) && (attackClient.hp < (s32)(attackClient.maxhp / 3)))
    // Fake Out / First Impression / Mat Block after user has already performed an action
    || ((currentMoveIndex == MOVE_FAKE_OUT || currentMoveIndex == MOVE_FIRST_IMPRESSION || currentMoveIndex == MOVE_MAT_BLOCK) && attackClient.moveeffect.fakeOutCount != ctx->total_turn)
    // Follow Me / Rage Powder in singles
    || ((currentMoveIndex == MOVE_FOLLOW_ME || currentMoveIndex == MOVE_RAGE_POWDER) && !(BattleTypeGet(bsys) & (BATTLE_TYPE_MULTI | BATTLE_TYPE_DOUBLE)))
    // Future Sight / Doom Desire into target that already has future attack
    || (moveEffect == MOVE_EFFECT_HIT_IN_3_TURNS && (ctx->fcc.future_prediction_count[ctx->defence_client] != 0))
    // Poltergeist when the target does not have an item
    || (currentMoveIndex == MOVE_POLTERGEIST && defenceClient.item == ITEM_NONE)
    // Sleep Talk / Snore while not asleep
    || ((moveEffect == MOVE_EFFECT_USE_RANDOM_LEARNED_MOVE_SLEEP || moveEffect == MOVE_EFFECT_DAMAGE_WHILE_ASLEEP) && !(attackClient.condition & STATUS_SLEEP))
    // Rest while user is already asleep
    || (moveEffect == MOVE_EFFECT_RECOVER_HEALTH_AND_SLEEP && attackClient.condition & STATUS_SLEEP)
    // Steel Roller when there is no Terrain
    || (currentMoveIndex == MOVE_STEEL_ROLLER && ctx->terrainOverlay.type == TERRAIN_NONE)
    // Stuff Cheeks when user doesn't have a Berry
    || (currentMoveIndex == MOVE_STUFF_CHEEKS && !IS_ITEM_BERRY(attackClient.item))
    // Stockpile with 3 Stockpiles already
    || (moveEffect == MOVE_EFFECT_STOCKPILE && attackClient.moveeffect.stockpileCount == 3)
    // Swallow / Spit Up with 0 Stockpiles
    || ((moveEffect == MOVE_EFFECT_SWALLOW || moveEffect == MOVE_EFFECT_SPIT_UP) && attackClient.moveeffect.stockpileCount == 0)
    // Last Resort when user has not used all its other moves once or user does not have Last Resort in its moveslot
    || (moveEffect == MOVE_EFFECT_FAIL_IF_NOT_USED_ALL_OTHER_MOVES && (ctx->battlemon[ctx->attack_client].moveeffect.lastResortCount < cnt - 1 || cnt < 2))
    // Sucker Punch when target doesn't have an eligible move pending
    || (moveEffect == MOVE_EFFECT_HIT_FIRST_IF_TARGET_ATTACKING && (ctx->playerActions[ctx->defence_client][3] == CONTROLLER_COMMAND_40 || (ctx->moveTbl[move].power == 0 && !ctx->oneTurnFlag[ctx->defence_client].struggle_flag)))
    // Teleport with nothing to switch to
    || (moveEffect == MOVE_EFFECT_FLEE_FROM_WILD_BATTLE && (BattleTypeGet(bsys) & BATTLE_TYPE_TRAINER) && !CanSwitchMon(bsys, ctx, ctx->attack_client))
    // TODO: Magic Room
    // Fling / Natural Gift: Embargo or Magic Room are in effect, or ineligible held item, or no item
    || ((moveEffect == MOVE_EFFECT_FLING || moveEffect == MOVE_EFFECT_NATURAL_GIFT) && attackClient.moveeffect.embargoFlag)
    ) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->server_seq_no = CONTROLLER_COMMAND_25;
        ctx->waza_status_flag |= MOVE_STATUS_FLAG_FAILED;
        return TRUE;
    }

    // Counter / Mirror Coat / Metal Burst when user hasn't been damaged
    // TODO: Copied from BtlCmd_Counter, BtlCmd_TryMetalBurst, contradicts existing documentation
    int battlerId;
    BOOL flag = FALSE;

    int sideA = IsClientEnemy(bsys, ctx->attack_client);
    int sideB = 0;

    switch (ctx->current_move_index) {
        case MOVE_COUNTER:
            battlerId = ctx->oneTurnFlag[ctx->attack_client].physical_damager;
            sideB = IsClientEnemy(bsys, battlerId);
            if (!(ctx->oneTurnFlag[ctx->attack_client].physical_damage[battlerId] && sideA != sideB && ctx->battlemon[battlerId].hp)) {
                flag = TRUE;
                ctx->oneSelfFlag[ctx->attack_client].no_pressure_flag = 1;
            }
            break;
        case MOVE_MIRROR_COAT:
            battlerId = ctx->oneTurnFlag[ctx->attack_client].special_damager;
            sideB = IsClientEnemy(bsys, battlerId);
            if (!(ctx->oneTurnFlag[ctx->attack_client].special_damage[battlerId] && sideA != sideB && ctx->battlemon[battlerId].hp)) {
                ctx->oneSelfFlag[ctx->attack_client].no_pressure_flag = 1;
            }
            break;
        case MOVE_METAL_BURST:
            sideB = IsClientEnemy(bsys, ctx->oneTurnFlag[ctx->attack_client].last_damage);
            if (!(ctx->oneTurnFlag[ctx->attack_client].last_damage && sideA != sideB && ctx->battlemon[ctx->oneTurnFlag[ctx->attack_client].last_damager].hp)) {
                flag = TRUE;
            }
            break;
        default:
            break;
    }

    if (flag) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->server_seq_no = CONTROLLER_COMMAND_25;
        ctx->waza_status_flag |= MOVE_STATUS_FLAG_FAILED;
        return TRUE;
    }
    // Weight moves into Dynamax
    if (IsWeightMove(currentMoveIndex) && defenceClient.is_currently_dynamaxed) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_CANT_USE_MOVE_DYNAMAX_TARGET);
        ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
        return TRUE;
    }

    // TODO: Destiny Bond when user has "Destiny Bond" Y-info volatile
    // TODO: No Retreat when user has Can't Escape flag set by No Retreat
    // TODO: Refactor random roll location - Protecting move when user failed to repeat a successive protecting move
    // TODO: Protecting move when move is the last used in the turn

    // Following has to be in order

    if (moveEffect == MOVE_EFFECT_RECOVER_HEALTH_AND_SLEEP) {
        // Rest while user is at full HP
        if (attackClient.hp == (s32)attackClient.maxhp) {
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_RESTORE_HP_FULL_FAIL);
            ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
            return TRUE;
        }
        // Rest while user has Insomnia / Vital Spirit
        if (attackerAbility == ABILITY_INSOMNIA || attackerAbility == ABILITY_VITAL_SPIRIT) {
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
            // TODO: test
            ctx->battlerIdTemp = ctx->attack_client;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_STAYED_AWAKE);
            ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
            return TRUE;
        }
    }
    return FALSE;
}

void BattleController_CheckBide(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (ctx->battlemon[ctx->attack_client].condition2 & STATUS2_BIDE) {
        ctx->battlemon[ctx->attack_client].condition2 -= (1 << STATUS2_BIDE_SHIFT);
        if (!(ctx->battlemon[ctx->attack_client].condition2 & STATUS2_BIDE) && ctx->store_damage[ctx->attack_client]) {
            ctx->damage = ctx->store_damage[ctx->attack_client] * 2;
            if (ctx->battlemon[ctx->client_no_hit[ctx->attack_client]].hp != 0) {
                ctx->defence_client = ctx->client_no_hit[ctx->attack_client];
            } else {
                ctx->defence_client = Battler_GetRandomOpposingBattlerId(bsys, ctx, ctx->attack_client);
                if (ctx->battlemon[ctx->defence_client].hp == 0) {
                    LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_BIDE_END_NO_TARGET);
                    ctx->next_server_seq_no = CONTROLLER_COMMAND_39;
                    ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                }
            }
        }
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_END_BIDE);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
    }
}

BOOL BattleController_CheckAbilityFailures1(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx) {
    int attacker = ctx->attack_client;
    int defender = ctx->defence_client;

    // Handle Damp
    switch (ctx->current_move_index) {
    case MOVE_SELF_DESTRUCT:
    case MOVE_EXPLOSION:
    case MOVE_MIND_BLOWN:
    case MOVE_MISTY_EXPLOSION:
        if (CheckSideAbility(gBattleSystem, ctx, CHECK_ABILITY_ALL_HP, defender, ABILITY_DAMP)
        && CLIENT_DOES_NOT_HAVE_MOLD_BREAKER_VARIATIONS(ctx, attacker)) {
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_CANNOT_USE_MOVE);
            ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
            return TRUE;
        }
        break;
    
    default:
        break;
    }
    
    // Handle Queenly Majesty, Dazzling & Armor Tail
    if ((CheckSideAbility(gBattleSystem, ctx, CHECK_ABILITY_SAME_SIDE_HP, defender, ABILITY_QUEENLY_MAJESTY)
    || CheckSideAbility(gBattleSystem, ctx, CHECK_ABILITY_SAME_SIDE_HP, defender, ABILITY_DAZZLING)
    || CheckSideAbility(gBattleSystem, ctx, CHECK_ABILITY_SAME_SIDE_HP, defender, ABILITY_ARMOR_TAIL))
    && CLIENT_DOES_NOT_HAVE_MOLD_BREAKER_VARIATIONS(ctx, attacker)) {
        if (AdjustedMoveHasPositivePriority(ctx, attacker) && CurrentMoveShouldNotBeExemptedFromPriorityBlocking(ctx, attacker, defender)) {
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_CANNOT_USE_MOVE);
            ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
            return TRUE;
        }
    }

    return FALSE;
}

// TODO: Handle Pledge moves
BOOL BattleController_CheckInterruptibleMoves(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx) {
    int effect = ctx->moveTbl[ctx->current_move_index].effect;
    if (effect == MOVE_EFFECT_HIT_IN_3_TURNS) {
        ctx->server_seq_no = CONTROLLER_COMMAND_25;
        return TRUE;
    }
    return FALSE;
}

// TODO: Check correctness
// TODO: Implement new mechanics
BOOL BattleController_CheckChargeMoves(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;

    // affected by Power Herb
    switch (moveEffect) {
    case MOVE_EFFECT_CHARGE_TURN_HIGH_CRIT:
    case MOVE_EFFECT_CHARGE_TURN_HIGH_CRIT_FLINCH:
    case MOVE_EFFECT_CHARGE_TURN_DEF_UP:
    case MOVE_EFFECT_CHARGE_TURN_SUN_SKIPS:
    case MOVE_EFFECT_FLY:
    case MOVE_EFFECT_DIVE:
    case MOVE_EFFECT_DIG:
    case MOVE_EFFECT_BOUNCE:
    case MOVE_EFFECT_SHADOW_FORCE:
    case MOVE_EFFECT_CHARGE_TURN_ATK_SP_ATK_SPEED_UP_2:
    case MOVE_EFFECT_CHARGE_TURN_SP_ATK_UP_RAIN_SKIPS:
    case MOVE_EFFECT_CHARGE_TURN_SP_ATK_UP:
        if (!(ctx->battlemon[ctx->attack_client].condition2 & STATUS2_LOCKED_INTO_MOVE) && GET_HELD_ITEM_HOLD_EFFECT_ACCOUNTING_KLUTZ(ctx, ctx->attack_client) != HOLD_EFFECT_CHARGE_SKIP) {
            ctx->server_seq_no = CONTROLLER_COMMAND_24;
        return TRUE;
        }
        break;
    
    default:
        break;
    }

    // not affected by Power Herb
    switch (moveEffect) {
    case MOVE_EFFECT_BIDE:
    // case MOVE_EFFECT_SKY_DROP:
        if (!(ctx->battlemon[ctx->attack_client].condition2 & STATUS2_LOCKED_INTO_MOVE)) {
            ctx->server_seq_no = CONTROLLER_COMMAND_24;
        return TRUE;
        }
        break;
    
    default:
        break;
    }

    // need to BATTLE_SUBSCRIPT_CHARGE_MOVE_CLEANUP if fail below checks???
    return FALSE;
}

BOOL BattleController_CheckStolenBySnatch(struct BattleSystem *bw UNUSED, struct BattleStruct *sp) {
    int i;
    int client_no;
    int client_set_max;

    client_set_max = BattleWorkClientSetMaxGet(bw);

    if (sp->defence_client == 0xFF) {
        return FALSE;
    }

    for (i = 0; i < client_set_max; i++) {
        client_no = sp->turnOrder[i];
        if (((sp->waza_status_flag & 0x801FDA49) == 0) && (sp->oneTurnFlag[client_no].snatchFlag) && (sp->moveTbl[sp->current_move_index].flag & FLAG_SNATCH)) {
            sp->battlerIdTemp = client_no;
            sp->oneTurnFlag[client_no].snatchFlag = 0;
            if ((sp->server_status_flag & (0x00100000)) == 0) {
                sp->waza_no_mamoru[sp->attack_client] = 0;
                sp->waza_no_old[sp->attack_client] = sp->moveNoTemp;
                sp->waza_no_last = sp->moveNoTemp;
                sp->server_status_flag |= (0x00100000);
            }
            LoadBattleSubSeqScript(sp, 1, SUB_SEQ_SNATCH);
            sp->next_server_seq_no = sp->server_seq_no;
            sp->server_seq_no = 22;
            CheckPressureForPPDecrease(sp, client_no, sp->attack_client);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL BattleController_CheckSemiInvulnerability(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    if (!(ctx->waza_status_flag & MOVE_STATUS_FLAG_LOCK_ON)
    && !(GetBattlerAbility(ctx, ctx->attack_client) != ABILITY_NO_GUARD)
    && ctx->moveTbl[ctx->current_move_index].target != MOVE_TARGET_OPPONENTS_FIELD
    && (
        (!(ctx->server_status_flag & BATTLE_STATUS_HIT_FLY) && ctx->battlemon[defender].effect_of_moves & MOVE_EFFECT_FLAG_FLYING_IN_AIR)
        || (!(ctx->server_status_flag & BATTLE_STATUS_SHADOW_FORCE) && ctx->battlemon[defender].effect_of_moves & MOVE_EFFECT_FLAG_SHADOW_FORCE)
        || (!(ctx->server_status_flag & BATTLE_STATUS_HIT_DIG) && ctx->battlemon[defender].effect_of_moves & MOVE_EFFECT_FLAG_DIGGING)
        || (!(ctx->server_status_flag & BATTLE_STATUS_HIT_DIVE) && ctx->battlemon[defender].effect_of_moves & MOVE_EFFECT_FLAG_IS_DIVING)
        )) {
        ctx->moveStatusFlagForSpreadMoves[defender] = WAZA_STATUS_FLAG_KIE_NOHIT;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_ATTACK_MISSED);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}


BOOL BattleController_CheckProtect(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    if (ctx->oneTurnFlag[defender].mamoru_flag
        && ctx->moveTbl[ctx->current_move_index].flag & (1 << 1)
        && (ctx->current_move_index != MOVE_CURSE || CurseUserIsGhost(ctx, ctx->current_move_index, ctx->attack_client) == TRUE)
        && (!CheckMoveIsChargeMove(ctx, ctx->current_move_index) || ctx->server_status_flag & BATTLE_STATUS_CHARGE_MOVE_HIT)) {
        UnlockBattlerOutOfCurrentMove(bsys, ctx, ctx->attack_client);
        ctx->battlerIdTemp = defender;
        ctx->moveStatusFlagForSpreadMoves[defender] = WAZA_STATUS_FLAG_MAMORU_NOHIT;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_PROTECTED);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleController_CheckPsychicTerrain(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    // Handle Psychic Terrain
    // Block any natural priority move or a move made priority by an ability, if the terrain is Psychic Terrain
    // Courtesy of Dray (https://github.com/Drayano60)
    if (ctx->terrainOverlay.type == PSYCHIC_TERRAIN && ctx->terrainOverlay.numberOfTurnsLeft > 0 && MoldBreakerIsClientGrounded(ctx, ctx->attack_client, defender)) {
        if (AdjustedMoveHasPositivePriority(ctx, ctx->attack_client) && CurrentMoveShouldNotBeExemptedFromPriorityBlocking(ctx, ctx->attack_client, defender) && ((ctx->attack_client & 1) != (defender & 1))) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->battlerIdTemp = defender;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_NO_MORE_WORK;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_HANDLE_PSYCHIC_TERRAIN_PROTECTION);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
        }
    }
    return FALSE;
}

// TODO: Handle Smack Down, Ingrain
BOOL BattleController_CheckTelekinesis(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    // TODO: this to my knowledge handle base species, and does not consider transformed species, which is correct. Requires verifying
    int defenderSpecies = ctx->battlemon[defender].species;
    int defenderForm = ctx->battlemon[defender].form_no;
    if (((defenderSpecies == SPECIES_GENGAR && defenderForm == 1) || defenderSpecies == SPECIES_DIGLETT || defenderSpecies == SPECIES_DUGTRIO || defenderSpecies == SPECIES_SANDYGAST || defenderSpecies == SPECIES_PALOSSAND)
    || ctx->battlemon[defender].effect_of_moves & MOVE_EFFECT_FLAG_INGRAIN) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
        ctx->battlerIdTemp = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_BUT_IT_FAILED_SPREAD);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

BOOL CalcDamageAndSetMoveStatusFlags(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender) {
    if ((ctx->moveTbl[ctx->current_move_index].target != MOVE_TARGET_USER && ctx->moveTbl[ctx->current_move_index].target != MOVE_TARGET_USER_SIDE && ctx->moveTbl[ctx->current_move_index].power != 0 && !(ctx->server_status_flag & BATTLE_STATUS_IGNORE_TYPE_IMMUNITY) && !(ctx->server_status_flag & BATTLE_STATUS_CHARGE_TURN)) || ctx->current_move_index == MOVE_THUNDER_WAVE) {
        // TODO: Probably wrong?
        u32 temp = ctx->moveStatusFlagForSpreadMoves[defender];
        ServerDoTypeCalcMod(bsys, ctx, ctx->current_move_index, ctx->move_type, ctx->attack_client, defender, ctx->damageForSpreadMoves[defender], &temp);
        ctx->moveStatusFlagForSpreadMoves[defender] = temp;
        if (ctx->moveStatusFlagForSpreadMoves[defender] & MOVE_STATUS_FLAG_NOT_EFFECTIVE) {
            ctx->moveOutCheck[ctx->attack_client].stoppedFromIneffective = TRUE;
        }
    }
    return FALSE;
}

// TODO: check message/subscript correctness, move damage out
BOOL BattleController_CheckTypeImmunity(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender) {
    if (!(ctx->waza_out_check_on_off & 2) && ctx->defence_client != 0xFF && CalcDamageAndSetMoveStatusFlags(bsys, ctx, defender) == TRUE) {
        return FALSE;
    }
    if (ctx->moveStatusFlagForSpreadMoves[defender] & MOVE_STATUS_FLAG_NOT_EFFECTIVE) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->battlerIdTemp = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_DOESNT_AFFECT);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleController_CheckLevitate(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    if ((MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_LEVITATE) == TRUE) && (ctx->move_type == TYPE_GROUND)
        // iron ball halves speed and grounds
        && (GET_HELD_ITEM_HOLD_EFFECT_ACCOUNTING_KLUTZ(ctx, defender) != HOLD_EFFECT_SPEED_DOWN_GROUNDED)) {
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_LEVITATE_MISS;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_LEVITATE_FAIL);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

// TODO: Handle Telekinesis
BOOL BattleController_CheckAirBalloonTelekinesisMagnetRise(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    if (
        (
            (ctx->battlemon[defender].moveeffect.magnetRiseTurns)
            && ((ctx->battlemon[defender].effect_of_moves & MOVE_EFFECT_FLAG_INGRAIN) == 0)
            && ((ctx->field_condition & FIELD_STATUS_GRAVITY) == 0)
            && (ctx->move_type == TYPE_GROUND)
            && (GET_HELD_ITEM_HOLD_EFFECT_ACCOUNTING_KLUTZ(ctx, defender) != HOLD_EFFECT_SPEED_DOWN_GROUNDED)
        )
        ||
        (
            (GET_HELD_ITEM_HOLD_EFFECT_ACCOUNTING_KLUTZ(ctx, defender) == HOLD_EFFECT_UNGROUND_DESTROYED_ON_HIT) // has air balloon
            && ((ctx->battlemon[defender].effect_of_moves & MOVE_EFFECT_FLAG_INGRAIN) == 0)
            && ((ctx->field_condition & FIELD_STATUS_GRAVITY) == 0)
            && (ctx->move_type == TYPE_GROUND)
        )
       ) {
        // TODO: if in the future the AI somehow needs to read this flag, create a new flag for Air Balloon
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_MAGNET_RISE_MISS;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_DOESNT_AFFECT);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleController_CheckSafetyGoggles(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    if (IsPowderMove(ctx->current_move_index) && GET_HELD_ITEM_HOLD_EFFECT_ACCOUNTING_KLUTZ(ctx, ctx->defence_client) == HOLD_EFFECT_SPORE_POWDER_IMMUNITY) {
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_NOT_EFFECTIVE;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_SAFETY_GOGGLES);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

// TODO: modernise message
BOOL BattleController_CheckAbilityFailures3(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    // Handle Bulletproof
    if ((MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_BULLETPROOF) && IsBallOrBombMove(ctx->current_move_index))
    // Handle Sticky Hold
    || (MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_STICKY_HOLD) && (ctx->current_move_index == MOVE_TRICK || ctx->current_move_index == MOVE_SWITCHEROO || ctx->current_move_index == MOVE_CORROSIVE_GAS))) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_SOUNDPROOF);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleController_CheckTypeBasedMoveConditionImmunities1(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;
    int priority = GetClientActionPriority(bsys, ctx, ctx->attack_client);

    // Dark-type Prankster immunity
    if ((priority > 0 && GetBattlerAbility(ctx, ctx->attack_client) == ABILITY_PRANKSTER && HasType(ctx, defender, TYPE_DARK) && (ctx->attack_client & 1) != (defender & 1)) // used on an enemy)
    // Ghost-type immunity to trapping moves
    // TODO: handle Octolock
    || (moveEffect == MOVE_EFFECT_PREVENT_ESCAPE && HasType(ctx, defender, TYPE_GHOST))
    // Grass-type powder immunity
    || (IsPowderMove(ctx->current_move_index) && HasType(ctx, defender, TYPE_GRASS))
    // Ice-type immunity to Sheer Cold
    || (ctx->current_move_index == MOVE_SHEER_COLD && HasType(ctx, defender, TYPE_ICE))) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_NOT_EFFECTIVE;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_DOESNT_AFFECT);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

// TODO: Check message correctness
BOOL BattleController_CheckMoveFailures2(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;

    // Attract into a target with same gender / genderless
    if ((moveEffect == MOVE_EFFECT_INFATUATE && ctx->battlemon[ctx->attack_client].sex == ctx->battlemon[defender].sex)
    // TODO: Check message correctness
    // If move is Captivate, check for gender immunity
    || (moveEffect == MOVE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER
    && ((ctx->battlemon[ctx->attack_client].sex == ctx->battlemon[defender].sex) || ctx->battlemon[ctx->attack_client].sex == POKEMON_GENDER_UNKNOWN || MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_OBLIVIOUS)))
    // Torment into Dynamax
    || (moveEffect == MOVE_EFFECT_TORMENT && ctx->battlemon[defender].is_currently_dynamaxed)) {
        ctx->waza_status_flag |= MOVE_STATUS_FLAG_NOT_EFFECTIVE;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->server_seq_no = CONTROLLER_COMMAND_25;
        return TRUE;
    }

    return FALSE;
}

// TODO: implement new mechanics
int BattleController_CheckMoveFailures2_VenomDrench(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx UNUSED, int defender UNUSED) {
    // return SUB_SEQ_BUT_IT_FAILED_SPREAD;
    return 0;
}

BOOL BattleController_CheckWhirlwindFailures(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    if (ctx->moveTbl[ctx->current_move_index].effect == MOVE_EFFECT_FORCE_SWITCH) {
        // 1. Handle Dynamax
        if (ctx->battlemon[defender].is_currently_dynamaxed) {
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
            ctx->msg_work = defender;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_FORCE_SWITCH_FAIL_DYNAMAX);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->moveStatusFlagForSpreadMoves[defender]= MOVE_STATUS_NO_MORE_WORK;
            return TRUE;
        }

        // 2. Handle Suction Cups
        if (MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_SUCTION_CUPS)) {
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
            ctx->msg_work = defender;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_FORCE_SWITCH_FAIL_SUCTION_CUPS);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_NO_MORE_WORK;
            return TRUE;
        }

        // 3. Handle Ingrain
        if (ctx->battlemon[defender].effect_of_moves & MOVE_EFFECT_FLAG_INGRAIN) {
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
            ctx->msg_work = defender;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_FORCE_SWITCH_FAIL_INGRAIN);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_NO_MORE_WORK;
            return TRUE;
        }
    }

    return FALSE;
}

BOOL BattleController_CheckTypeBasedMoveConditionImmunities2(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;

    // Electric-type paralysis immunity
    if ((moveEffect == MOVE_EFFECT_STATUS_PARALYZE && HasType(ctx, defender, TYPE_ELECTRIC))
    // Fire-type burn immunity
    || (moveEffect == MOVE_EFFECT_STATUS_BURN && HasType(ctx, defender, TYPE_FIRE))
    // Grass-type Leech Seed immunity
    || (moveEffect == MOVE_EFFECT_STATUS_LEECH_SEED && HasType(ctx, defender, TYPE_GRASS))
    // Poison / Steel-type poison / badly poison immunity
    || ((moveEffect == MOVE_EFFECT_STATUS_POISON || moveEffect == MOVE_EFFECT_STATUS_BADLY_POISON) && HasType(ctx, defender, TYPE_POISON) && GetBattlerAbility(ctx, ctx->attack_client) != ABILITY_CORROSION)) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_NOT_EFFECTIVE;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_DOESNT_AFFECT);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleController_CheckUproarStoppingSleepMoves(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;

    if (ctx->field_condition & FIELD_STATUS_UPROAR && moveEffect == MOVE_EFFECT_STATUS_SLEEP) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_UPROAR_STOPPING_SLEEP_MOVES);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }

    if (ctx->field_condition & FIELD_STATUS_UPROAR && moveEffect == MOVE_EFFECT_STATUS_SLEEP_NEXT_TURN) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_UPROAR_STOPPING_REST);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleController_CheckSafeguard(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    if (ctx->side_condition[IsClientEnemy(bsys, defender)] & SIDE_STATUS_SAFEGUARD
    && (GetBattlerAbility(ctx, ctx->attack_client) != ABILITY_INFILTRATOR)) {
        switch (ctx->moveTbl[ctx->current_move_index].effect) {
        case MOVE_EFFECT_STATUS_SLEEP:
        case MOVE_EFFECT_STATUS_SLEEP_NEXT_TURN:
        case MOVE_EFFECT_STATUS_PARALYZE:
        case MOVE_EFFECT_STATUS_POISON:
        case MOVE_EFFECT_STATUS_BADLY_POISON:
        case MOVE_EFFECT_STATUS_BURN:
        case MOVE_EFFECT_STATUS_CONFUSE:
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
            ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
            ctx->msg_work = defender;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_PROTECTED_BY_SAFEGUARD);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            return TRUE;
            break;
        
        default:
            break;
        }
    }
    return FALSE;
}

BOOL BattleController_CheckTerrainBlock(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;
    // TODO: Come back after terrain refactor
    if (ctx->terrainOverlay.numberOfTurnsLeft > 0) {
        switch (ctx->terrainOverlay.type) {
            case ELECTRIC_TERRAIN:
                if ((moveEffect == MOVE_EFFECT_STATUS_SLEEP || moveEffect == MOVE_EFFECT_STATUS_SLEEP_NEXT_TURN) && MoldBreakerIsClientGrounded(ctx, ctx->attack_client, defender)) {
                    ctx->msg_work = defender;
                    LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_ELECTRIC_TERRAIN_PROTECTION);
                    ctx->next_server_seq_no = ctx->server_seq_no;
                    ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                    ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
                    return TRUE;
                }
                break;

            case MISTY_TERRAIN:
                if ((moveEffect == MOVE_EFFECT_STATUS_SLEEP || moveEffect == MOVE_EFFECT_STATUS_SLEEP_NEXT_TURN || moveEffect == MOVE_EFFECT_STATUS_PARALYZE || moveEffect == MOVE_EFFECT_STATUS_POISON || moveEffect == MOVE_EFFECT_STATUS_BADLY_POISON || moveEffect == MOVE_EFFECT_STATUS_BURN || moveEffect == MOVE_EFFECT_STATUS_CONFUSE) && MoldBreakerIsClientGrounded(ctx, ctx->attack_client, defender)) {
                    ctx->msg_work = defender;
                    LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_MISTY_TERRAIN_PROTECTION);
                    ctx->next_server_seq_no = ctx->server_seq_no;
                    ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                    ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
                    return TRUE;
                }
                break;
            default:
                break;
        }
    }
    return FALSE;
}

// TODO: Implement mew mechanics
int BattlerController_CheckSubstituteBlockingStatDropsOrDecorate(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;
    if (GetBattlerAbility(ctx, ctx->attack_client) != ABILITY_INFILTRATOR && ctx->battlemon[defender].condition2 & STATUS2_SUBSTITUTE) {
        if (ctx->attack_client != defender) {
            switch (moveEffect) {
                case MOVE_EFFECT_ATK_DOWN:
                case MOVE_EFFECT_DEF_DOWN:
                case MOVE_EFFECT_SPEED_DOWN:
                case MOVE_EFFECT_SP_ATK_DOWN:
                case MOVE_EFFECT_SP_DEF_DOWN:
                case MOVE_EFFECT_ACC_DOWN:
                case MOVE_EFFECT_EVA_DOWN:
                case MOVE_EFFECT_ATK_DOWN_2:
                case MOVE_EFFECT_DEF_DOWN_2:
                case MOVE_EFFECT_SPEED_DOWN_2:
                case MOVE_EFFECT_SP_ATK_DOWN_2:
                case MOVE_EFFECT_SP_DEF_DOWN_2:
                case MOVE_EFFECT_ACC_DOWN_2:
                case MOVE_EFFECT_EVA_DOWN_2:
                case MOVE_EFFECT_ATK_DEF_DOWN:
                case MOVE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER:
                case MOVE_EFFECT_ATK_DOWN_3:
                case MOVE_EFFECT_DEF_DOWN_3:
                case MOVE_EFFECT_SPEED_DOWN_3:
                case MOVE_EFFECT_SP_ATK_DOWN_3:
                case MOVE_EFFECT_SP_DEF_DOWN_3:
                    return SUB_SEQ_DOESNT_AFFECT;
                    break;

                default:
                    return 0;
                    break;
            }
        }
    }

    return 0;
}

int BattlerController_CheckMist(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;
    if (ctx->attack_client != defender) {
        switch (moveEffect) {
            case MOVE_EFFECT_ATK_DOWN:
            case MOVE_EFFECT_DEF_DOWN:
            case MOVE_EFFECT_SPEED_DOWN:
            case MOVE_EFFECT_SP_ATK_DOWN:
            case MOVE_EFFECT_SP_DEF_DOWN:
            case MOVE_EFFECT_ACC_DOWN:
            case MOVE_EFFECT_EVA_DOWN:
            case MOVE_EFFECT_ATK_DOWN_2:
            case MOVE_EFFECT_DEF_DOWN_2:
            case MOVE_EFFECT_SPEED_DOWN_2:
            case MOVE_EFFECT_SP_ATK_DOWN_2:
            case MOVE_EFFECT_SP_DEF_DOWN_2:
            case MOVE_EFFECT_ACC_DOWN_2:
            case MOVE_EFFECT_EVA_DOWN_2:
            case MOVE_EFFECT_ATK_DEF_DOWN:
            case MOVE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER:
            case MOVE_EFFECT_ATK_DOWN_3:
            case MOVE_EFFECT_DEF_DOWN_3:
            case MOVE_EFFECT_SPEED_DOWN_3:
            case MOVE_EFFECT_SP_ATK_DOWN_3:
            case MOVE_EFFECT_SP_DEF_DOWN_3:
                // infiltrator bypasses mist
                if (ctx->scw[IsClientEnemy(bsys, defender)].mistCount && GetBattlerAbility(ctx, ctx->attack_client) != ABILITY_INFILTRATOR) {
                    return BATTLE_MSG_PROTECTED_BY_MIST;
                }
                break;

            default:
                return 0;
                break;
        }
    }
    return 0;
}

BOOL BattleController_CheckAbilityFailures4_StatBasedFailures(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;
    BOOL hasClearBodyOrFullMetalBodyOrWhiteSmoke = MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_CLEAR_BODY) || MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_FULL_METAL_BODY) || MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_WHITE_SMOKE);

    // TODO: Check correctness
    BOOL hasFlowerVeil = MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_FLOWER_VEIL) || MoldBreakerAbilityCheck(ctx, ctx->attack_client, BATTLER_ALLY(defender), ABILITY_FLOWER_VEIL);

    int subscriptToRun = 0;

    switch (moveEffect) {
    case MOVE_EFFECT_ATK_DOWN:
    case MOVE_EFFECT_ATK_DOWN_2:
        if (MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_HYPER_CUTTER)) {
            subscriptToRun = SUB_SEQ_ATTACK_NOT_LOWERED;
            break;
        }
        if (hasFlowerVeil) {
            subscriptToRun = SUB_SEQ_FLOWER_VEIL_FAIL;
            break;
        }
        if (hasClearBodyOrFullMetalBodyOrWhiteSmoke) {
            subscriptToRun = SUB_SEQ_STATS_NOT_LOWERED;
            break;
        }
        break;
    case MOVE_EFFECT_DEF_DOWN:
    case MOVE_EFFECT_DEF_DOWN_2:
        if (MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_BIG_PECKS)) {
            subscriptToRun = SUB_SEQ_DEFENSE_NOT_LOWERED;
            break;
        }
        if (hasFlowerVeil) {
            subscriptToRun = SUB_SEQ_FLOWER_VEIL_FAIL;
            break;
        }
        if (hasClearBodyOrFullMetalBodyOrWhiteSmoke) {
            subscriptToRun = SUB_SEQ_STATS_NOT_LOWERED;
            break;
        }
        break;
    case MOVE_EFFECT_ATK_DEF_DOWN:  // Tickle
        // If the move is Tickle, first attack will drop, then Big Pecks will prevent the Defense drop.
        // If the move is Tickle, first Hyper Cutter will block the Attack drop, then Defense will drop.

        if (hasFlowerVeil) {
            subscriptToRun = SUB_SEQ_FLOWER_VEIL_FAIL;
            break;
        }
        if (hasClearBodyOrFullMetalBodyOrWhiteSmoke) {
            subscriptToRun = SUB_SEQ_STATS_NOT_LOWERED;
            break;
        }
        break;
    case MOVE_EFFECT_SPEED_DOWN:
    case MOVE_EFFECT_SPEED_DOWN_2:
        if (hasFlowerVeil) {
            subscriptToRun = SUB_SEQ_FLOWER_VEIL_FAIL;
            break;
        }
        if (hasClearBodyOrFullMetalBodyOrWhiteSmoke) {
            subscriptToRun = SUB_SEQ_STATS_NOT_LOWERED;
            break;
        }
        break;
    case MOVE_EFFECT_SP_ATK_DOWN:
    case MOVE_EFFECT_SP_ATK_DOWN_2:
    case MOVE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER:
        if (hasFlowerVeil) {
            subscriptToRun = SUB_SEQ_FLOWER_VEIL_FAIL;
            break;
        }
        if (hasClearBodyOrFullMetalBodyOrWhiteSmoke) {
            subscriptToRun = SUB_SEQ_STATS_NOT_LOWERED;
            break;
        }
        break;
    case MOVE_EFFECT_SP_DEF_DOWN:
    case MOVE_EFFECT_SP_DEF_DOWN_2:
        if (hasFlowerVeil) {
            subscriptToRun = SUB_SEQ_FLOWER_VEIL_FAIL;
            break;
        }
        if (hasClearBodyOrFullMetalBodyOrWhiteSmoke) {
            subscriptToRun = SUB_SEQ_STATS_NOT_LOWERED;
            break;
        }
        break;
    case MOVE_EFFECT_ACC_DOWN:
    case MOVE_EFFECT_ACC_DOWN_2:
        if (MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_KEEN_EYE)) {
            subscriptToRun = SUB_SEQ_ACCURACY_NOT_LOWERED;
            break;
        }
        if (hasFlowerVeil) {
            subscriptToRun = SUB_SEQ_FLOWER_VEIL_FAIL;
            break;
        }
        if (hasClearBodyOrFullMetalBodyOrWhiteSmoke) {
            subscriptToRun = SUB_SEQ_STATS_NOT_LOWERED;
            break;
        }
        break;
    case MOVE_EFFECT_EVA_DOWN:
    case MOVE_EFFECT_EVA_DOWN_2:
        if (hasFlowerVeil) {
            subscriptToRun = SUB_SEQ_FLOWER_VEIL_FAIL;
            break;
        }
        if (hasClearBodyOrFullMetalBodyOrWhiteSmoke) {
            subscriptToRun = SUB_SEQ_STATS_NOT_LOWERED;
            break;
        }
        break;
    default:
        subscriptToRun = 0;
        break;
    }

    if (subscriptToRun) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, subscriptToRun);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_NO_MORE_WORK;
        return TRUE;
    }
    return FALSE;
}

// TODO: Implement new mechanics
BOOL BattleController_CheckAbilityFailures4_StatusBasedFailures(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;
    int attacker = ctx->attack_client;
    BOOL doChecking = FALSE;
    BOOL hasFlowerVeil = FALSE;
    BOOL hasPastelVeil = FALSE;
    BOOL doesNotAffect = FALSE;
    BOOL ShieldsDownCanActivate = (MoldBreakerAbilityCheck(ctx, attacker, defender, ABILITY_SHIELDS_DOWN) || (ctx->battlemon[defender].species == SPECIES_MINIOR && ctx->battlemon[defender].form_no == 1));

    // TODO: Check correctness
    hasFlowerVeil = MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_FLOWER_VEIL) || MoldBreakerAbilityCheck(ctx, ctx->attack_client, BATTLER_ALLY(defender), ABILITY_FLOWER_VEIL);
    hasPastelVeil = MoldBreakerAbilityCheck(ctx, ctx->attack_client, defender, ABILITY_PASTEL_VEIL) || MoldBreakerAbilityCheck(ctx, ctx->attack_client, BATTLER_ALLY(defender), ABILITY_PASTEL_VEIL);

    if (hasFlowerVeil) {
        switch (moveEffect) {
            case MOVE_EFFECT_STATUS_SLEEP:
            case MOVE_EFFECT_STATUS_SLEEP_NEXT_TURN:
            case MOVE_EFFECT_RECOVER_DAMAGE_SLEEP:
            case MOVE_EFFECT_STATUS_PARALYZE:
            case MOVE_EFFECT_STATUS_POISON:
            case MOVE_EFFECT_STATUS_BADLY_POISON:
            case MOVE_EFFECT_STATUS_BURN:
                ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
                ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
                ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
                ctx->battlerIdTemp = defender;
                LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_FLOWER_VEIL_FAIL);
                ctx->next_server_seq_no = ctx->server_seq_no;
                ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                return TRUE;
                break;

            default:
                break;
        }
    }

    // No Stomping Tantrum doubling
    if (ShieldsDownCanActivate || MoldBreakerAbilityCheck(ctx, attacker, defender, ABILITY_LEAF_GUARD)) {
        if (moveEffect == MOVE_EFFECT_RECOVER_DAMAGE_SLEEP) {
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
            ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
            ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
            ctx->battlerIdTemp = defender;
            LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_DOESNT_AFFECT_ABILITY);
            ctx->next_server_seq_no = ctx->server_seq_no;
            ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
            return TRUE;
        }
    }

    // TODO: implement Sweet Veil (Rest)

    if (MoldBreakerAbilityCheck(ctx, attacker, defender, ABILITY_COMATOSE)
    || MoldBreakerAbilityCheck(ctx, attacker, defender, ABILITY_LEAF_GUARD)
    || ShieldsDownCanActivate) {
        doChecking = TRUE;
    }

    if ((MoldBreakerAbilityCheck(ctx, attacker, defender, ABILITY_IMMUNITY) || hasPastelVeil)
    && (moveEffect == MOVE_EFFECT_STATUS_POISON || moveEffect == MOVE_EFFECT_STATUS_BADLY_POISON)) {
        doesNotAffect = TRUE;
    }

    // TODO: implement Sweet Veil
    if ((MoldBreakerAbilityCheck(ctx, attacker, defender, ABILITY_INSOMNIA))
    && (moveEffect == MOVE_EFFECT_STATUS_SLEEP || moveEffect == MOVE_EFFECT_STATUS_SLEEP_NEXT_TURN || moveEffect == MOVE_EFFECT_RECOVER_HEALTH_AND_SLEEP)) {
        doesNotAffect = TRUE;
    }

    if (MoldBreakerAbilityCheck(ctx, attacker, defender, ABILITY_LIMBER) && (moveEffect == MOVE_EFFECT_STATUS_PARALYZE)) {
        doesNotAffect = TRUE;
    }

    if (MoldBreakerAbilityCheck(ctx, attacker, defender, ABILITY_OBLIVIOUS) && (moveEffect == MOVE_EFFECT_INFATUATE || moveEffect == MOVE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER)) {
        doesNotAffect = TRUE;
    }

    if (MoldBreakerAbilityCheck(ctx, attacker, defender, ABILITY_OWN_TEMPO) && (moveEffect == MOVE_EFFECT_STATUS_CONFUSE)) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
        ctx->battlerIdTemp = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_OWN_TEMPO_FAIL);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }

    if ((MoldBreakerAbilityCheck(ctx, attacker, defender, ABILITY_WATER_BUBBLE) || MoldBreakerAbilityCheck(ctx, attacker, defender, ABILITY_WATER_VEIL))
    && (moveEffect == MOVE_EFFECT_STATUS_BURN)) {
        doesNotAffect = TRUE;
    }

    if (doChecking) {
        switch (moveEffect) {
            case MOVE_EFFECT_STATUS_SLEEP:
            case MOVE_EFFECT_STATUS_SLEEP_NEXT_TURN:
            case MOVE_EFFECT_RECOVER_DAMAGE_SLEEP:
            case MOVE_EFFECT_STATUS_PARALYZE:
            case MOVE_EFFECT_STATUS_POISON:
            case MOVE_EFFECT_STATUS_BADLY_POISON:
            case MOVE_EFFECT_STATUS_BURN:
                doesNotAffect = TRUE;
                break;

            default:
                break;
        }
    }

    if (doesNotAffect) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
        ctx->battlerIdTemp = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_DOESNT_AFFECT_ABILITY);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }

    return FALSE;
}

// TODO: Implement new mechanics
BOOL BattleController_CheckAbilityFailures4_OtherAromaVeilSturdy(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;

    if (MoldBreakerAbilityCheck(ctx, ctx->attack_client, ctx->defence_client, ABILITY_STURDY) && moveEffect == MOVE_EFFECT_ONE_HIT_KO) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->moveStatusFlagForSpreadMoves[ctx->defence_client] = MOVE_STATUS_FLAG_FAILED;
        ctx->battlerIdTemp = ctx->defence_client;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_DOESNT_AFFECT_ABILITY);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleController_CheckMoveAccuracy(struct BattleSystem *bsys, struct BattleStruct *ctx, int defender) {
    if (!(ctx->waza_out_check_on_off & 0x20) && defender != BATTLER_NONE && BattleSystem_CheckMoveHit(bsys, ctx, ctx->attack_client, defender, ctx->current_move_index) == TRUE) {
        return FALSE;
    }
    if (!(ctx->waza_out_check_on_off & 0x40) && defender != BATTLER_NONE && BattleSystem_CheckMoveEffect(bsys, ctx, ctx->attack_client, defender, ctx->current_move_index) == TRUE) {
        return FALSE;
    }

    if (ctx->waza_status_flag & MOVE_STATUS_FLAG_MISS) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->waza_status_flag = 0;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_MISS;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_ATTACK_MISSED);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleController_CheckSubstituteBlockingOtherEffects(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;
    if (ctx->battlemon[ctx->defence_client].condition2 & STATUS2_SUBSTITUTE) {
        if (ctx->attack_client != ctx->defence_client) {
            switch (moveEffect) {
                // TODO: Handle Electrify, Flower Shield, Forest's Curse, Trick-or-Treat, Magic Powder, Heal Pulse, Purify, Tar Shot, Topsy-Turvy

                // List of effects tested: attempting to inflict a major status condition, Acupressure, Block / Mean Look / Spider Web / Octolock, Electrify / Quash, Flower Shield, Forest's Curse / Trick-or-Treat / Soak / Magic Powder, Gastro Acid, Guard Split / Power Split, Heal Pulse, Leech Seed, Lock-On, Mind Reader, Pain Split, Psycho Shift, Purify, Simple Beam, Tar Shot, Topsy-Turvy, Transform, Worry Seed
                case MOVE_EFFECT_RANDOM_STAT_UP_2:
                case MOVE_EFFECT_PREVENT_ESCAPE:
                case MOVE_EFFECT_QUASH:
                case MOVE_EFFECT_CHANGE_TO_WATER_TYPE:
                case MOVE_EFFECT_SUPRESS_ABILITY:
                case MOVE_EFFECT_GUARD_SPLIT:
                case MOVE_EFFECT_POWER_SPLIT:
                case MOVE_EFFECT_STATUS_LEECH_SEED:
                case MOVE_EFFECT_NEXT_ATTACK_ALWAYS_HITS:
                case MOVE_EFFECT_AVERAGE_HP:
                case MOVE_EFFECT_TRANSFER_STATUS:
                case MOVE_EFFECT_SET_ABILITY_TO_SIMPLE:
                    if (GetBattlerAbility(ctx, ctx->attack_client) != ABILITY_INFILTRATOR) {
                        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_BUT_IT_FAILED_SPREAD);
                        ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
                        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
                        return TRUE;
                                        }
                    break;
                // TODO: Handle Sky Drop here
                case MOVE_EFFECT_TRANSFORM:
                case MOVE_EFFECT_SET_ABILITY_TO_INSOMNIA:
                    LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_BUT_IT_FAILED_SPREAD);
                    ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
                    ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                    ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
                    return TRUE;
                    break;

                default:
                    return FALSE;
                    break;
            }
        }
    }

    return FALSE;
}

BOOL BattleController_CheckMoveFailures5(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;

    int attackerCondition = ctx->battlemon[ctx->attack_client].condition;

    switch (moveEffect) {
        // Psycho Shift
        case MOVE_EFFECT_TRANSFER_STATUS:
            // Electric-type paralysis immunity
            if ((attackerCondition & STATUS_PARALYSIS && HasType(ctx, defender, TYPE_ELECTRIC))
                // Fire-type burn immunity
                || (attackerCondition & STATUS_PARALYSIS && HasType(ctx, defender, TYPE_ELECTRIC))
                // Poison / Steel-type poison / badly poison immunity
                || (attackerCondition & STATUS_POISON_ALL && (HasType(ctx, defender, TYPE_POISON) || HasType(ctx, defender, TYPE_STEEL)))) {
                ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_NOT_EFFECTIVE;
                ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
                ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
                ctx->msg_work = defender;
                LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_DOESNT_AFFECT);
                ctx->next_server_seq_no = ctx->server_seq_no;
                ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                return TRUE;
            }
            break;
        // Substitute
        case MOVE_EFFECT_SET_SUBSTITUTE:
            if (ctx->battlemon[ctx->attack_client].hp <= BattleDamageDivide(ctx->battlemon[ctx->attack_client].maxhp, 4)) {
                ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
                ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
                ctx->msg_work = defender;
                LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_SUBSTITUTE_FAIL);
                ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
                ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
                return TRUE;
            }
            break;
        default:
            break;
    }
    return FALSE;
}

// TODO: implement new mechanics
BOOL BattleController_CheckAromaVeil(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx UNUSED, int defender UNUSED) {
    return FALSE;
}

BOOL IfAllClientsHavePerishSong(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    int battlerId;

    int maxBattlers = BattleWorkClientSetMaxGet(bsys);

    int cnt = 0;

    for (battlerId = 0; battlerId < maxBattlers; battlerId++) {
        if (ctx->battlemon[battlerId].effect_of_moves & MOVE_EFFECT_FLAG_PERISH_SONG_ACTIVE || ctx->battlemon[battlerId].hp == 0 /* || MoldBreakerAbilityCheck(ctx, ctx->attack_client, battlerId, ABILITY_SOUNDPROOF) == TRUE */) {
            cnt++;
        }
    }
    if (cnt == maxBattlers) {
        return TRUE;
    }

    return FALSE;
}

// TODO: implement new mechanics
BOOL BattleController_CheckMoveFailures3(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;

    // But it failed
    // Attract into a target already infatuated
    if ((moveEffect == MOVE_EFFECT_INFATUATE && ctx->battlemon[defender].condition2 & STATUS2_ATTRACT)
    // Ingrain when user already has Ingrain
    || (moveEffect == MOVE_EFFECT_GROUND_TRAP_USER_CONTINUOUS_HEAL && ctx->battlemon[ctx->attack_client].effect_of_moves & MOVE_EFFECT_FLAG_INGRAIN)
    // Leech Seed when target already has Leech Seed
    || (moveEffect == MOVE_EFFECT_STATUS_LEECH_SEED && ctx->battlemon[defender].effect_of_moves & MOVE_EFFECT_FLAG_LEECH_SEED_ACTIVE)
    // Status move into target that already has another major status condition (but not the same one)
    || ((moveEffect == MOVE_EFFECT_STATUS_SLEEP || moveEffect == MOVE_EFFECT_STATUS_SLEEP_NEXT_TURN || moveEffect == MOVE_EFFECT_STATUS_PARALYZE || moveEffect == MOVE_EFFECT_STATUS_POISON || moveEffect == MOVE_EFFECT_STATUS_BADLY_POISON || moveEffect == MOVE_EFFECT_STATUS_BURN) && ctx->battlemon[defender].condition)
    // Torment when target already has Torment
    || (moveEffect == MOVE_EFFECT_TORMENT && ctx->battlemon[defender].effect_of_moves & MOVE_EFFECT_FLAG_EMBARGO)
    // Yawn into target with status condition / already has Yawn
    || (moveEffect == MOVE_EFFECT_STATUS_SLEEP_NEXT_TURN && (ctx->battlemon[defender].effect_of_moves & MOVE_EFFECT_YAWN_COUNTER || ctx->battlemon[defender].condition))
    // Worry Seed when target has Insomnia / Truant
    || (!IfAbilityCanBeReplacedByWorrySeed(ctx, ctx->attack_client))
    // TODO: Coaching in singles or when there is no ally target available in doubles
    ) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->msg_work = defender;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_BUT_IT_FAILED_SPREAD);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }

    // It doesn't affect xxx
    // Dream Eater when target is awake
    if ((moveEffect == MOVE_EFFECT_RECOVER_DAMAGE_SLEEP && !((ctx->battlemon[defender].condition & STATUS_SLEEP) || GetBattlerAbility(ctx, defender)))
    // Endeavor when target has equal or less HP than the user
    || (moveEffect == MOVE_EFFECT_SET_HP_EQUAL_TO_USER && ctx->battlemon[ctx->attack_client].hp <= ctx->battlemon[defender].hp)) {
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_NOT_EFFECTIVE;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_DOESNT_AFFECT);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }

    // xxx is unaffected
    // OHKO moves against a target with higher level than the user / target is Dynamaxed
    if (moveEffect == MOVE_EFFECT_ONE_HIT_KO && (ctx->battlemon[ctx->attack_client].level < ctx->battlemon[defender].level || ctx->battlemon[defender].is_currently_dynamaxed)) {
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_OHKO_HIT_NOHIT;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_UNAFFECTED);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }

    // xxx is already xxx
    // Status move into target that already has that status
    if ((moveEffect == MOVE_EFFECT_STATUS_SLEEP && ctx->battlemon[defender].condition & STATUS_SLEEP)
    || (moveEffect == MOVE_EFFECT_STATUS_SLEEP_NEXT_TURN && ctx->battlemon[defender].condition & MOVE_EFFECT_YAWN_COUNTER)
    || (moveEffect == MOVE_EFFECT_STATUS_PARALYZE && ctx->battlemon[defender].condition & STATUS_PARALYSIS)
    || ((moveEffect == MOVE_EFFECT_STATUS_POISON || moveEffect == MOVE_EFFECT_STATUS_BADLY_POISON) && ctx->battlemon[defender].condition & STATUS_POISON_ALL)
    || (moveEffect == MOVE_EFFECT_STATUS_BURN && ctx->battlemon[defender].condition & STATUS_BURN)) {
        ctx->moveStatusFlagForSpreadMoves[defender] = MOVE_STATUS_FLAG_FAILED;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_flag = 0;
        ctx->oneTurnFlag[ctx->attack_client].parental_bond_is_active = FALSE;
        ctx->msg_work = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_ALREADY_HAS_SAME_STATUS);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }

    // TODO: Check fail message in game
    // Synchronoise: non-matching type

    return FALSE;
}

BOOL BattleController_CheckMoveFailures3_PerishSong(struct BattleSystem *bsys, struct BattleStruct *ctx) {
    if (IfAllClientsHavePerishSong(bsys, ctx) && ctx->current_move_index == MOVE_PERISH_SONG) {
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_CANNOT_USE_MOVE);
        ctx->next_server_seq_no = CONTROLLER_COMMAND_25;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        ctx->waza_status_flag |= MOVE_STATUS_NO_MORE_WORK;
        return TRUE;
    }
    return FALSE;
}

// TODO: implement new mechanics
// Only return true if no stats are changed
BOOL BattleController_CheckMoveFailures3_StatsChanges(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    int moveEffect = ctx->moveTbl[ctx->current_move_index].effect;
    switch (moveEffect) {
        case MOVE_EFFECT_ATK_UP:
        case MOVE_EFFECT_ATK_UP_2:
        case MOVE_EFFECT_ATK_UP_3:
        case MOVE_EFFECT_ATK_UP_2_STATUS_CONFUSION:
            if (ctx->battlemon[defender].states[STAT_ATTACK] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_DEF_UP:
        case MOVE_EFFECT_DEF_UP_2:
        case MOVE_EFFECT_DEF_UP_3:
        case MOVE_EFFECT_DEF_UP_DOUBLE_ROLLOUT_POWER:
        // TODO: Stuff Cheeks with Defense maxed out
            if (ctx->battlemon[defender].states[STAT_DEFENSE] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_SPEED_UP:
        case MOVE_EFFECT_SPEED_UP_2:
        case MOVE_EFFECT_SPEED_UP_3:
            if (ctx->battlemon[defender].states[STAT_SPEED] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_SP_ATK_UP:
        case MOVE_EFFECT_SP_ATK_UP_2:
        case MOVE_EFFECT_SP_ATK_UP_3:
        case MOVE_EFFECT_SP_ATK_UP_CAUSE_CONFUSION:
            if (ctx->battlemon[defender].states[STAT_SPATK] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_SP_DEF_UP:
        case MOVE_EFFECT_SP_DEF_UP_2:
        case MOVE_EFFECT_SP_DEF_UP_3:
        case MOVE_EFFECT_SP_DEF_UP_DOUBLE_ELECTRIC_POWER:
            if (ctx->battlemon[defender].states[STAT_SPDEF] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_ACC_UP:
        case MOVE_EFFECT_ACC_UP_2:
            if (ctx->battlemon[defender].states[STAT_ACCURACY] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_EVA_UP:
        case MOVE_EFFECT_EVA_UP_2:
        case MOVE_EFFECT_EVA_UP_2_MINIMIZE:
            if (ctx->battlemon[defender].states[STAT_EVASION] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_ATK_DEF_UP:
            if (ctx->battlemon[defender].states[STAT_ATTACK] == 12 && ctx->battlemon[defender].states[STAT_DEFENSE] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_STOCKPILE:
        case MOVE_EFFECT_DEF_SP_DEF_UP:
            if (ctx->battlemon[defender].states[STAT_DEFENSE] == 12 && ctx->battlemon[defender].states[STAT_SPDEF] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_SP_ATK_SP_DEF_UP:
            if (ctx->battlemon[defender].states[STAT_SPATK] == 12 && ctx->battlemon[defender].states[STAT_SPDEF] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_ATK_SPEED_UP:
        case MOVE_EFFECT_SPEED_UP_2_ATK_UP:
            if (ctx->battlemon[defender].states[STAT_ATTACK] == 12 && ctx->battlemon[defender].states[STAT_SPEED] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_RANDOM_STAT_UP_2:
        // TODO: Clangorous Soul / No Retreat with all stats maxed out
            if (ctx->battlemon[defender].states[STAT_ATTACK] == 12
            && ctx->battlemon[defender].states[STAT_DEFENSE] == 12
            && ctx->battlemon[defender].states[STAT_SPEED] == 12
            && ctx->battlemon[defender].states[STAT_SPATK] == 12
            && ctx->battlemon[defender].states[STAT_SPDEF] == 12
            && ctx->battlemon[defender].states[STAT_ACCURACY] == 12
            && ctx->battlemon[defender].states[STAT_EVASION] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_ATK_ACC_UP:
            if (ctx->battlemon[defender].states[STAT_ATTACK] == 12 && ctx->battlemon[defender].states[STAT_ACCURACY] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_SP_ATK_SP_DEF_SPEED_UP:
            if (ctx->battlemon[defender].states[STAT_SPEED] == 12
            && ctx->battlemon[defender].states[STAT_SPATK] == 12
            && ctx->battlemon[defender].states[STAT_SPDEF] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_ATK_DEF_ACC_UP:
            if (ctx->battlemon[defender].states[STAT_ATTACK] == 12
            && ctx->battlemon[defender].states[STAT_DEFENSE] == 12
            && ctx->battlemon[defender].states[STAT_ACCURACY] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_ATK_SP_ATK_SPEED_UP_2_DEF_SP_DEF_DOWN:
        case MOVE_EFFECT_ATK_SP_ATK_SPEED_UP_2:
            if (ctx->battlemon[defender].states[STAT_ATTACK] == 12
            && ctx->battlemon[defender].states[STAT_SPEED] == 12
            && ctx->battlemon[defender].states[STAT_SPATK] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_ATK_SP_ATK_UP:
            if (ctx->battlemon[defender].states[STAT_ATTACK] == 12
            && ctx->battlemon[defender].states[STAT_SPATK] == 12) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_ATK_DOWN:
        case MOVE_EFFECT_ATK_DOWN_2:
        case MOVE_EFFECT_ATK_DOWN_3:
            if (ctx->battlemon[defender].states[STAT_ATTACK] == 0) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_DEF_DOWN:
        case MOVE_EFFECT_DEF_DOWN_2:
        case MOVE_EFFECT_DEF_DOWN_3:
            if (ctx->battlemon[defender].states[STAT_DEFENSE] == 0) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_SPEED_DOWN:
        case MOVE_EFFECT_SPEED_DOWN_2:
        case MOVE_EFFECT_SPEED_DOWN_3:
            if (ctx->battlemon[defender].states[STAT_SPEED] == 0) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_SP_ATK_DOWN:
        case MOVE_EFFECT_SP_ATK_DOWN_2:
        case MOVE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER:
        case MOVE_EFFECT_SP_ATK_DOWN_3:
            if (ctx->battlemon[defender].states[STAT_SPATK] == 0) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_SP_DEF_DOWN:
        case MOVE_EFFECT_SP_DEF_DOWN_2:
        case MOVE_EFFECT_SP_DEF_DOWN_3:
            if (ctx->battlemon[defender].states[STAT_SPDEF] == 0) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_ACC_DOWN:
        case MOVE_EFFECT_ACC_DOWN_2:
            if (ctx->battlemon[defender].states[STAT_ACCURACY] == 0) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_EVA_DOWN:
        case MOVE_EFFECT_EVA_DOWN_2:
            if (ctx->battlemon[defender].states[STAT_EVASION] == 0) {
                return TRUE;
            }
            break;
        case MOVE_EFFECT_ATK_DEF_DOWN:
            if (ctx->battlemon[defender].states[STAT_ATTACK] == 0 && ctx->battlemon[defender].states[STAT_DEFENSE] == 0) {
                return TRUE;
            }
            break;
        default:
            break;
    }
    return FALSE;
}

/**
 *  @brief checks if the given move should be weakened or not (only prints message)
 *  @param bw battle work structure
 *  @param sp global battle structure
 *  @return TRUE/FALSE
 */
BOOL BattleController_CheckStrongWindsWeaken(struct BattleSystem *bw, struct BattleStruct *sp, int defender) {
    int defender_type_1 = BattlePokemonParamGet(sp, defender, BATTLE_MON_DATA_TYPE1, NULL);
    int defender_type_2 = BattlePokemonParamGet(sp, defender, BATTLE_MON_DATA_TYPE2, NULL);
    u32 move_type = GetAdjustedMoveType(sp, sp->attack_client, sp->current_move_index);
    int i = 0;

    // TODO: Check type3
    while (TypeEffectivenessTable[i][0] != 0xff) {
        if (TypeEffectivenessTable[i][0] == move_type) {
            if ((TypeEffectivenessTable[i][1] == defender_type_1) || (TypeEffectivenessTable[i][1] == defender_type_2)) {
                if ((!CheckSideAbility(bw, sp, CHECK_ABILITY_ALL_HP, 0, ABILITY_CLOUD_NINE) && !CheckSideAbility(bw, sp, CHECK_ABILITY_ALL_HP, 0, ABILITY_AIR_LOCK)
                && sp->field_condition & WEATHER_STRONG_WINDS
                && (TypeEffectivenessTable[i][2] == 20)
                && ((defender_type_1 == TYPE_FLYING) || (defender_type_2 == TYPE_FLYING)))) {
                    LoadBattleSubSeqScript(sp, ARC_BATTLE_SUB_SEQ, SUB_SEQ_WEAKEN_MOVES_STRONG_WINDS);
                    sp->next_server_seq_no = sp->server_seq_no;
                    sp->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
                    return TRUE;
                }
            }
        }
        i++;
    }

    return FALSE;
}

// TODO: implement new mechanics
BOOL BattleController_CheckTeraShell(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx UNUSED, int defender UNUSED) {
    return FALSE;
}

BOOL BattleController_TryConsumeDamageReductionBerry(struct BattleSystem *bsys UNUSED, struct BattleStruct *ctx, int defender) {
    if (CanActivateDamageReductionBerry(bsys, ctx, defender)) {
        ctx->battlerIdTemp = defender;
        LoadBattleSubSeqScript(ctx, ARC_BATTLE_SUB_SEQ, SUB_SEQ_PLAY_EAT_BERRY_ANIMATION);
        ctx->next_server_seq_no = ctx->server_seq_no;
        ctx->server_seq_no = CONTROLLER_COMMAND_RUN_SCRIPT;
        return TRUE;
    }
    return FALSE;
}
