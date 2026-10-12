# Boundary evidence for the additional ov030 batch

These are provisional working TUs, not a claim to recover the original file boundaries.
The rows below preserve the relevant findings from the local 2026-10-08 TU review.
The proposed data intervals were separately checked against all configured owners on
main and the pinned field branch before adoption. No disputed BSS was adopted.

A+ combines a task boundary and candidate data layout evidence. A uses the task
convention. C is a data-layout candidate. Edge means the start of the scanned gap,
not an independently graded original source-file edge. Confidence in a boundary
is separate from confidence in exact code placement and data ownership.

Loaded bases alone cannot prove ownership: they may contain section addends or
reference external globals. Directly referenced strings are not necessarily
function-body literals. Compiler data sorting alone cannot prove an original TU.
The initial overlay-wide draft overlapped code/data owners; only the reviewed,
nonconflicting adoption intervals in manifest.json are used here.

| Original row | Text start | Text end | Functions | Confidence | Task | Evidence |
|---|---|---|---:|---|---|---|
| 0 | 0x020c420c | 0x020c45ec | 9 | edge | Tsk_Fld_TalkEventCursor | gap start (existing TU / section edge) |
| 0 | 0x020c420c | 0x020c45ec | 9 | edge | Tsk_Fld_TalkEventCursor | gap start (existing TU / section edge) |
| 1 | 0x020c45ec | 0x020c4850 | 9 | A+ | Tsk_Fld_TalkEventDecide | previous TU ends after the creator of Tsk_Fld_TalkEventCursor; data order ro(20e7260) |
| 2 | 0x020c4850 | 0x020c4a78 | 8 | A+ | Tsk_Fld_TalkEventTitle | previous TU ends after the creator of Tsk_Fld_TalkEventDecide; data order ro(20e72a8) |
| 3 | 0x020c4a78 | 0x020c5088 | 12 | A+ | Tsk_Fld_MapJumpName | previous TU ends after the creator of Tsk_Fld_TalkEventTitle; data order bs+da+ro(20e72f0,2107634,2109d70) @+2 |
| 4 | 0x020c5088 | 0x020c5404 | 10 | A+ | Tsk_Fld_MapName | previous TU ends after the creator of Tsk_Fld_MapJumpName; data order da+ro(2107648,20e7324) |
| 5 | 0x020c5404 | 0x020c548c | 3 | C |  | data order ro(20e7380) cost=0.0 (original data optimum interval 0x020c53e8-0x020c5404) |
| 7 | 0x020c5744 | 0x020c59b0 | 7 | A+ |  | previous TU ends after the creator of Tsk_Fld_NSymbolUGAppear; data order ro(20e7460); data order da+ro(20e9944,2108328) @+1 |
| 8 | 0x020c59b0 | 0x020c5f70 | 13 | C | Tsk_Fld_Piranha | data order da+ro(20e7f40,2108464) cost=2.0 (original data optimum interval 0x020c59b0-0x020c5a28) |
| 9 | 0x020c5f70 | 0x020c683c | 8 | A+ |  | previous TU ends after the creator of Tsk_Fld_Piranha (+2 API funcs); data order da+ro(2108490,20eae80) |
| 10 | 0x020c683c | 0x020c6c58 | 3 | C |  | data order bs+ro(20ec824,2109d98) cost=0.0 (original data optimum interval 0x020c683c-0x020c6aa4) |
| 11 | 0x020c6c58 | 0x020c6e90 | 8 | C | Tsk_Fld_TouchNext | data order ro(20ec844) cost=0.0 |
| 0 | 0x020c85a4 | 0x020c8868 | 8 | edge | Tsk_Fld_BattleIn | gap start (existing TU / section edge) |
| 1 | 0x020c8868 | 0x020c8a58 | 9 | A+ | Tsk_Fld_EventMail | previous TU ends after the creator of Tsk_Fld_BattleIn; data order bs+ro(2109d98,20ecab4) |
| 2 | 0x020c8a58 | 0x020c900c | 10 | A+ | Tsk_Fld_Brush | previous TU ends after the creator of Tsk_Fld_EventMail (+1 API funcs); data order da+ro(20ecafc,21085f0) @-1 |
| 3 | 0x020c900c | 0x020c91f4 | 8 | A+ | Tsk_Fld_BrushHati | previous TU ends after the creator of Tsk_Fld_Brush; data order da+ro(20ecb88,2108614) |
| 0 | 0x020ca1f4 | 0x020ca4bc | 10 | edge | Task_Mess | gap start (existing TU / section edge) |
| 0 | 0x020ca1f4 | 0x020ca4bc | 10 | edge | Task_Mess | gap start (existing TU / section edge) |
| 1 | 0x020ca4bc | 0x020cb12c | 10 | A+ |  | task Task_Mess; data order ro(20ecdc4) |
| 14 | 0x020d2a80 | 0x020d2f2c | 10 | C-base |  | noninterleaving accesses through .bss bases (0x2109de4 / 0x2109de8); ownership/addends unresolved (base-user separation interval 0x020d2620-0x020d2a80) |
| 16 | 0x020d317c | 0x020d3b3c | 16 | A+ |  | previous TU ends after the creator of Tsk_LocaTitle; data order ro(20ee880) @-51; data order da+ro(20ee998,2109110) |
