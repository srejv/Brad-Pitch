
#include "qmath.h"
#include "brad_pitch_instrumental.h"
#include "definebrad.h"
#include "brad_input.h"
#include <math.h>

Int32 env[PSHIFT_BUFSIZE] = {
0, 30, 61, 91, 121, 152, 
182, 213, 243, 273, 304, 334, 
364, 395, 425, 456, 486, 516, 
547, 577, 607, 638, 668, 698, 
729, 759, 790, 820, 850, 881, 
911, 941, 972, 1002, 1033, 1063, 
1093, 1124, 1154, 1184, 1215, 1245, 
1275, 1306, 1336, 1367, 1397, 1427, 
1458, 1488, 1518, 1549, 1579, 1610, 
1640, 1670, 1701, 1731, 1761, 1792, 
1822, 1853, 1883, 1913, 1944, 1974, 
2004, 2035, 2065, 2095, 2126, 2156, 
2187, 2217, 2247, 2278, 2308, 2338, 
2369, 2399, 2430, 2460, 2490, 2521, 
2551, 2581, 2612, 2642, 2672, 2703, 
2733, 2764, 2794, 2824, 2855, 2885, 
2915, 2946, 2976, 3007, 3037, 3067, 
3098, 3128, 3158, 3189, 3219, 3249, 
3280, 3310, 3341, 3371, 3401, 3432, 
3462, 3492, 3523, 3553, 3584, 3614, 
3644, 3675, 3705, 3735, 3766, 3796, 
3826, 3857, 3887, 3918, 3948, 3978, 
4009, 4039, 4069, 4100, 4130, 4161, 
4191, 4221, 4252, 4282, 4312, 4343, 
4373, 4403, 4434, 4464, 4495, 4525, 
4555, 4586, 4616, 4646, 4677, 4707, 
4738, 4768, 4798, 4829, 4859, 4889, 
4920, 4950, 4980, 5011, 5041, 5072, 
5102, 5132, 5163, 5193, 5223, 5254, 
5284, 5315, 5345, 5375, 5406, 5436, 
5466, 5497, 5527, 5558, 5588, 5618, 
5649, 5679, 5709, 5740, 5770, 5800, 
5831, 5861, 5892, 5922, 5952, 5983, 
6013, 6043, 6074, 6104, 6135, 6165, 
6195, 6226, 6256, 6286, 6317, 6347, 
6377, 6408, 6438, 6469, 6499, 6529, 
6560, 6590, 6620, 6651, 6681, 6712, 
6742, 6772, 6803, 6833, 6863, 6894, 
6924, 6954, 6985, 7015, 7046, 7076, 
7106, 7137, 7167, 7197, 7228, 7258, 
7289, 7319, 7349, 7380, 7410, 7440, 
7471, 7501, 7531, 7562, 7592, 7623, 
7653, 7683, 7714, 7744, 7774, 7805, 
7835, 7866, 7896, 7926, 7957, 7987, 
8017, 8048, 8078, 8108, 8139, 8169, 
8200, 8230, 8260, 8291, 8321, 8351, 
8382, 8412, 8443, 8473, 8503, 8534, 
8564, 8594, 8625, 8655, 8685, 8716, 
8746, 8777, 8807, 8837, 8868, 8898, 
8928, 8959, 8989, 9020, 9050, 9080, 
9111, 9141, 9171, 9202, 9232, 9263, 
9293, 9323, 9354, 9384, 9414, 9445, 
9475, 9505, 9536, 9566, 9597, 9627, 
9657, 9688, 9718, 9748, 9779, 9809, 
9840, 9870, 9900, 9931, 9961, 9991, 
10022, 10052, 10082, 10113, 10143, 10174, 
10204, 10234, 10265, 10295, 10325, 10356, 
10386, 10417, 10447, 10477, 10508, 10538, 
10568, 10599, 10629, 10659, 10690, 10720, 
10751, 10781, 10811, 10842, 10872, 10902, 
10933, 10963, 10994, 11024, 11054, 11085, 
11115, 11145, 11176, 11206, 11236, 11267, 
11297, 11328, 11358, 11388, 11419, 11449, 
11479, 11510, 11540, 11571, 11601, 11631, 
11662, 11692, 11722, 11753, 11783, 11813, 
11844, 11874, 11905, 11935, 11965, 11996, 
12026, 12056, 12087, 12117, 12148, 12178, 
12208, 12239, 12269, 12299, 12330, 12360, 
12390, 12421, 12451, 12482, 12512, 12542, 
12573, 12603, 12633, 12664, 12694, 12725, 
12755, 12785, 12816, 12846, 12876, 12907, 
12937, 12968, 12998, 13028, 13059, 13089, 
13119, 13150, 13180, 13210, 13241, 13271, 
13302, 13332, 13362, 13393, 13423, 13453, 
13484, 13514, 13545, 13575, 13605, 13636, 
13666, 13696, 13727, 13757, 13787, 13818, 
13848, 13879, 13909, 13939, 13970, 14000, 
14030, 14061, 14091, 14122, 14152, 14182, 
14213, 14243, 14273, 14304, 14334, 14364, 
14395, 14425, 14456, 14486, 14516, 14547, 
14577, 14607, 14638, 14668, 14699, 14729, 
14759, 14790, 14820, 14850, 14881, 14911, 
14941, 14972, 15002, 15033, 15063, 15093, 
15124, 15154, 15184, 15215, 15245, 15276, 
15306, 15336, 15367, 15397, 15427, 15458, 
15488, 15518, 15549, 15579, 15610, 15640, 
15670, 15701, 15731, 15761, 15792, 15822, 
15853, 15883, 15913, 15944, 15974, 16004, 
16035, 16065, 16095, 16126, 16156, 16187, 
16217, 16247, 16278, 16308, 16338, 16369, 
16399, 16430, 16460, 16490, 16521, 16551, 
16581, 16612, 16642, 16673, 16703, 16733, 
16764, 16794, 16824, 16855, 16885, 16915, 
16946, 16976, 17007, 17037, 17067, 17098, 
17128, 17158, 17189, 17219, 17250, 17280, 
17310, 17341, 17371, 17401, 17432, 17462, 
17492, 17523, 17553, 17584, 17614, 17644, 
17675, 17705, 17735, 17766, 17796, 17827, 
17857, 17887, 17918, 17948, 17978, 18009, 
18039, 18069, 18100, 18130, 18161, 18191, 
18221, 18252, 18282, 18312, 18343, 18373, 
18404, 18434, 18464, 18495, 18525, 18555, 
18586, 18616, 18646, 18677, 18707, 18738, 
18768, 18798, 18829, 18859, 18889, 18920, 
18950, 18981, 19011, 19041, 19072, 19102, 
19132, 19163, 19193, 19223, 19254, 19284, 
19315, 19345, 19375, 19406, 19436, 19466, 
19497, 19527, 19558, 19588, 19618, 19649, 
19679, 19709, 19740, 19770, 19800, 19831, 
19861, 19892, 19922, 19952, 19983, 20013, 
20043, 20074, 20104, 20135, 20165, 20195, 
20226, 20256, 20286, 20317, 20347, 20378, 
20408, 20438, 20469, 20499, 20529, 20560, 
20590, 20620, 20651, 20681, 20712, 20742, 
20772, 20803, 20833, 20863, 20894, 20924, 
20955, 20985, 21015, 21046, 21076, 21106, 
21137, 21167, 21197, 21228, 21258, 21289, 
21319, 21349, 21380, 21410, 21440, 21471, 
21501, 21532, 21562, 21592, 21623, 21653, 
21683, 21714, 21744, 21774, 21805, 21835, 
21866, 21896, 21926, 21957, 21987, 22017, 
22048, 22078, 22109, 22139, 22169, 22200, 
22230, 22260, 22291, 22321, 22351, 22382, 
22412, 22443, 22473, 22503, 22534, 22564, 
22594, 22625, 22655, 22686, 22716, 22746, 
22777, 22807, 22837, 22868, 22898, 22928, 
22959, 22989, 23020, 23050, 23080, 23111, 
23141, 23171, 23202, 23232, 23263, 23293, 
23323, 23354, 23384, 23414, 23445, 23475, 
23505, 23536, 23566, 23597, 23627, 23657, 
23688, 23718, 23748, 23779, 23809, 23840, 
23870, 23900, 23931, 23961, 23991, 24022, 
24052, 24083, 24113, 24143, 24174, 24204, 
24234, 24265, 24295, 24325, 24356, 24386, 
24417, 24447, 24477, 24508, 24538, 24568, 
24599, 24629, 24660, 24690, 24720, 24751, 
24781, 24811, 24842, 24872, 24902, 24933, 
24963, 24994, 25024, 25054, 25085, 25115, 
25145, 25176, 25206, 25237, 25267, 25297, 
25328, 25358, 25388, 25419, 25449, 25479, 
25510, 25540, 25571, 25601, 25631, 25662, 
25692, 25722, 25753, 25783, 25814, 25844, 
25874, 25905, 25935, 25965, 25996, 26026, 
26056, 26087, 26117, 26148, 26178, 26208, 
26239, 26269, 26299, 26330, 26360, 26391, 
26421, 26451, 26482, 26512, 26542, 26573, 
26603, 26633, 26664, 26694, 26725, 26755, 
26785, 26816, 26846, 26876, 26907, 26937, 
26968, 26998, 27028, 27059, 27089, 27119, 
27150, 27180, 27210, 27241, 27271, 27302, 
27332, 27362, 27393, 27423, 27453, 27484, 
27514, 27545, 27575, 27605, 27636, 27666, 
27696, 27727, 27757, 27788, 27818, 27848, 
27879, 27909, 27939, 27970, 28000, 28030, 
28061, 28091, 28122, 28152, 28182, 28213, 
28243, 28273, 28304, 28334, 28365, 28395, 
28425, 28456, 28486, 28516, 28547, 28577, 
28607, 28638, 28668, 28699, 28729, 28759, 
28790, 28820, 28850, 28881, 28911, 28942, 
28972, 29002, 29033, 29063, 29093, 29124, 
29154, 29184, 29215, 29245, 29276, 29306, 
29336, 29367, 29397, 29427, 29458, 29488, 
29519, 29549, 29579, 29610, 29640, 29670, 
29701, 29731, 29761, 29792, 29822, 29853, 
29883, 29913, 29944, 29974, 30004, 30035, 
30065, 30096, 30126, 30156, 30187, 30217, 
30247, 30278, 30308, 30338, 30369, 30399, 
30430, 30460, 30490, 30521, 30551, 30581, 
30612, 30642, 30673, 30703, 30733, 30764, 
30794, 30824, 30855, 30885, 30915, 30946, 
30976, 31007, 31037, 31067, 31098, 31128, 
31158, 31189, 31219, 31250, 31280, 31310, 
31341, 31371, 31401, 31432, 31462, 31493, 
31523, 31553, 31584, 31614, 31644, 31675, 
31705, 31735, 31766, 31796, 31827, 31857, 
31887, 31918, 31948, 31978, 32009, 32039, 
32070, 32100, 32130, 32161, 32191, 32221, 
32252, 32282, 32312, 32343, 32373, 32404, 
32434, 32464, 32495, 32525, 32555, 32586, 
32616, 32647, 32677, 32707, 32738, 32767, 
32767, 32738, 32707, 32677, 32647, 32616, 
32586, 32555, 32525, 32495, 32464, 32434, 
32404, 32373, 32343, 32312, 32282, 32252, 
32221, 32191, 32161, 32130, 32100, 32070, 
32039, 32009, 31978, 31948, 31918, 31887, 
31857, 31827, 31796, 31766, 31735, 31705, 
31675, 31644, 31614, 31584, 31553, 31523, 
31493, 31462, 31432, 31401, 31371, 31341, 
31310, 31280, 31250, 31219, 31189, 31158, 
31128, 31098, 31067, 31037, 31007, 30976, 
30946, 30915, 30885, 30855, 30824, 30794, 
30764, 30733, 30703, 30673, 30642, 30612, 
30581, 30551, 30521, 30490, 30460, 30430, 
30399, 30369, 30338, 30308, 30278, 30247, 
30217, 30187, 30156, 30126, 30096, 30065, 
30035, 30004, 29974, 29944, 29913, 29883, 
29853, 29822, 29792, 29761, 29731, 29701, 
29670, 29640, 29610, 29579, 29549, 29519, 
29488, 29458, 29427, 29397, 29367, 29336, 
29306, 29276, 29245, 29215, 29184, 29154, 
29124, 29093, 29063, 29033, 29002, 28972, 
28942, 28911, 28881, 28850, 28820, 28790, 
28759, 28729, 28699, 28668, 28638, 28607, 
28577, 28547, 28516, 28486, 28456, 28425, 
28395, 28365, 28334, 28304, 28273, 28243, 
28213, 28182, 28152, 28122, 28091, 28061, 
28030, 28000, 27970, 27939, 27909, 27879, 
27848, 27818, 27788, 27757, 27727, 27696, 
27666, 27636, 27605, 27575, 27545, 27514, 
27484, 27453, 27423, 27393, 27362, 27332, 
27302, 27271, 27241, 27210, 27180, 27150, 
27119, 27089, 27059, 27028, 26998, 26968, 
26937, 26907, 26876, 26846, 26816, 26785, 
26755, 26725, 26694, 26664, 26633, 26603, 
26573, 26542, 26512, 26482, 26451, 26421, 
26391, 26360, 26330, 26299, 26269, 26239, 
26208, 26178, 26148, 26117, 26087, 26056, 
26026, 25996, 25965, 25935, 25905, 25874, 
25844, 25814, 25783, 25753, 25722, 25692, 
25662, 25631, 25601, 25571, 25540, 25510, 
25479, 25449, 25419, 25388, 25358, 25328, 
25297, 25267, 25237, 25206, 25176, 25145, 
25115, 25085, 25054, 25024, 24994, 24963, 
24933, 24902, 24872, 24842, 24811, 24781, 
24751, 24720, 24690, 24660, 24629, 24599, 
24568, 24538, 24508, 24477, 24447, 24417, 
24386, 24356, 24325, 24295, 24265, 24234, 
24204, 24174, 24143, 24113, 24083, 24052, 
24022, 23991, 23961, 23931, 23900, 23870, 
23840, 23809, 23779, 23748, 23718, 23688, 
23657, 23627, 23597, 23566, 23536, 23505, 
23475, 23445, 23414, 23384, 23354, 23323, 
23293, 23263, 23232, 23202, 23171, 23141, 
23111, 23080, 23050, 23020, 22989, 22959, 
22928, 22898, 22868, 22837, 22807, 22777, 
22746, 22716, 22686, 22655, 22625, 22594, 
22564, 22534, 22503, 22473, 22443, 22412, 
22382, 22351, 22321, 22291, 22260, 22230, 
22200, 22169, 22139, 22109, 22078, 22048, 
22017, 21987, 21957, 21926, 21896, 21866, 
21835, 21805, 21774, 21744, 21714, 21683, 
21653, 21623, 21592, 21562, 21532, 21501, 
21471, 21440, 21410, 21380, 21349, 21319, 
21289, 21258, 21228, 21197, 21167, 21137, 
21106, 21076, 21046, 21015, 20985, 20955, 
20924, 20894, 20863, 20833, 20803, 20772, 
20742, 20712, 20681, 20651, 20620, 20590, 
20560, 20529, 20499, 20469, 20438, 20408, 
20378, 20347, 20317, 20286, 20256, 20226, 
20195, 20165, 20135, 20104, 20074, 20043, 
20013, 19983, 19952, 19922, 19892, 19861, 
19831, 19800, 19770, 19740, 19709, 19679, 
19649, 19618, 19588, 19558, 19527, 19497, 
19466, 19436, 19406, 19375, 19345, 19315, 
19284, 19254, 19223, 19193, 19163, 19132, 
19102, 19072, 19041, 19011, 18981, 18950, 
18920, 18889, 18859, 18829, 18798, 18768, 
18738, 18707, 18677, 18646, 18616, 18586, 
18555, 18525, 18495, 18464, 18434, 18404, 
18373, 18343, 18312, 18282, 18252, 18221, 
18191, 18161, 18130, 18100, 18069, 18039, 
18009, 17978, 17948, 17918, 17887, 17857, 
17827, 17796, 17766, 17735, 17705, 17675, 
17644, 17614, 17584, 17553, 17523, 17492, 
17462, 17432, 17401, 17371, 17341, 17310, 
17280, 17250, 17219, 17189, 17158, 17128, 
17098, 17067, 17037, 17007, 16976, 16946, 
16915, 16885, 16855, 16824, 16794, 16764, 
16733, 16703, 16673, 16642, 16612, 16581, 
16551, 16521, 16490, 16460, 16430, 16399, 
16369, 16338, 16308, 16278, 16247, 16217, 
16187, 16156, 16126, 16095, 16065, 16035, 
16004, 15974, 15944, 15913, 15883, 15853, 
15822, 15792, 15761, 15731, 15701, 15670, 
15640, 15610, 15579, 15549, 15518, 15488, 
15458, 15427, 15397, 15367, 15336, 15306, 
15276, 15245, 15215, 15184, 15154, 15124, 
15093, 15063, 15033, 15002, 14972, 14941, 
14911, 14881, 14850, 14820, 14790, 14759, 
14729, 14699, 14668, 14638, 14607, 14577, 
14547, 14516, 14486, 14456, 14425, 14395, 
14364, 14334, 14304, 14273, 14243, 14213, 
14182, 14152, 14122, 14091, 14061, 14030, 
14000, 13970, 13939, 13909, 13879, 13848, 
13818, 13787, 13757, 13727, 13696, 13666, 
13636, 13605, 13575, 13545, 13514, 13484, 
13453, 13423, 13393, 13362, 13332, 13302, 
13271, 13241, 13210, 13180, 13150, 13119, 
13089, 13059, 13028, 12998, 12968, 12937, 
12907, 12876, 12846, 12816, 12785, 12755, 
12725, 12694, 12664, 12633, 12603, 12573, 
12542, 12512, 12482, 12451, 12421, 12390, 
12360, 12330, 12299, 12269, 12239, 12208, 
12178, 12148, 12117, 12087, 12056, 12026, 
11996, 11965, 11935, 11905, 11874, 11844, 
11813, 11783, 11753, 11722, 11692, 11662, 
11631, 11601, 11571, 11540, 11510, 11479, 
11449, 11419, 11388, 11358, 11328, 11297, 
11267, 11236, 11206, 11176, 11145, 11115, 
11085, 11054, 11024, 10994, 10963, 10933, 
10902, 10872, 10842, 10811, 10781, 10751, 
10720, 10690, 10659, 10629, 10599, 10568, 
10538, 10508, 10477, 10447, 10417, 10386, 
10356, 10325, 10295, 10265, 10234, 10204, 
10174, 10143, 10113, 10082, 10052, 10022, 
9991, 9961, 9931, 9900, 9870, 9840, 
9809, 9779, 9748, 9718, 9688, 9657, 
9627, 9597, 9566, 9536, 9505, 9475, 
9445, 9414, 9384, 9354, 9323, 9293, 
9263, 9232, 9202, 9171, 9141, 9111, 
9080, 9050, 9020, 8989, 8959, 8928, 
8898, 8868, 8837, 8807, 8777, 8746, 
8716, 8685, 8655, 8625, 8594, 8564, 
8534, 8503, 8473, 8443, 8412, 8382, 
8351, 8321, 8291, 8260, 8230, 8200, 
8169, 8139, 8108, 8078, 8048, 8017, 
7987, 7957, 7926, 7896, 7866, 7835, 
7805, 7774, 7744, 7714, 7683, 7653, 
7623, 7592, 7562, 7531, 7501, 7471, 
7440, 7410, 7380, 7349, 7319, 7289, 
7258, 7228, 7197, 7167, 7137, 7106, 
7076, 7046, 7015, 6985, 6954, 6924, 
6894, 6863, 6833, 6803, 6772, 6742, 
6712, 6681, 6651, 6620, 6590, 6560, 
6529, 6499, 6469, 6438, 6408, 6377, 
6347, 6317, 6286, 6256, 6226, 6195, 
6165, 6135, 6104, 6074, 6043, 6013, 
5983, 5952, 5922, 5892, 5861, 5831, 
5800, 5770, 5740, 5709, 5679, 5649, 
5618, 5588, 5558, 5527, 5497, 5466, 
5436, 5406, 5375, 5345, 5315, 5284, 
5254, 5223, 5193, 5163, 5132, 5102, 
5072, 5041, 5011, 4980, 4950, 4920, 
4889, 4859, 4829, 4798, 4768, 4738, 
4707, 4677, 4646, 4616, 4586, 4555, 
4525, 4495, 4464, 4434, 4403, 4373, 
4343, 4312, 4282, 4252, 4221, 4191, 
4161, 4130, 4100, 4069, 4039, 4009, 
3978, 3948, 3918, 3887, 3857, 3826, 
3796, 3766, 3735, 3705, 3675, 3644, 
3614, 3584, 3553, 3523, 3492, 3462, 
3432, 3401, 3371, 3341, 3310, 3280, 
3249, 3219, 3189, 3158, 3128, 3098, 
3067, 3037, 3007, 2976, 2946, 2915, 
2885, 2855, 2824, 2794, 2764, 2733, 
2703, 2672, 2642, 2612, 2581, 2551, 
2521, 2490, 2460, 2430, 2399, 2369, 
2338, 2308, 2278, 2247, 2217, 2187, 
2156, 2126, 2095, 2065, 2035, 2004, 
1974, 1944, 1913, 1883, 1853, 1822, 
1792, 1761, 1731, 1701, 1670, 1640, 
1610, 1579, 1549, 1518, 1488, 1458, 
1427, 1397, 1367, 1336, 1306, 1275, 
1245, 1215, 1184, 1154, 1124, 1093, 
1063, 1033, 1002, 972, 941, 911, 
881, 850, 820, 790, 759, 729, 
698, 668, 638, 607, 577, 547, 
516, 486, 456, 425, 395, 364, 
334, 304, 273, 243, 213, 182, 
152, 121, 91, 61, 30, 0
};

#include "brad_pitch_cent_table.inc"

fixedp pitch_delay[4000];

// t = this. ;)
void process_pitchshift(PShift *t, fixedp *process) {
	
	int rpi, ep, i, j;
	fixedp s = 0, rpf, frac, next, p;
	// 2^n/12
	p = qpow(short2q(2), qmul(short2q(t->pitch), Q1_12));

	// g�r detta antagande. 
	// cent = 2^cent/1200, 2^n/12 * 2^cent/1200 = 2^(n/12)+(cent/1200)
	// Kommentera bort raden under f�r att ta bprt cent. inte s�ker p� att den st�mmer dock!
	p = qmul(p, pitch_table_cent[t->cent + 100]);

	for(i = 0; i < PROCESS_SIZE; i++) {
		// taps loop (taps = 2)
		for(j = 0; j < PSHIFT_TAPS; j++) {
			// tap position, offset
			rpf = t->rp + j*(t->qdsize)/PSHIFT_TAPS;

			rpf = rpf < t->qdsize ? rpf : rpf - t->qdsize;
			rpi = qipart(rpf);
			frac = qfpart(rpf);
			next = (rpi != t->dsize-1 ? pitch_delay[rpi+1] : pitch_delay[0]);
			// envelope index
			ep = rpi - t->wp;
			if(ep < 0) ep += t->dsize;
			s += qmul(pitch_delay[rpi] + qmul(frac, (next - pitch_delay[rpi])), env[ep]);
		}
		// increment reader pointer and check bounds
		t->rp += p;
		t->rp = t->rp < t->qdsize ? t->rp : t->rp - t->qdsize;
		// feed the delay line
		pitch_delay[t->wp] = process[i] + qmul(s,t->fdb);
		// output the signal
		process[i] = qadd(qmul(qmul( s / PSHIFT_TAPS, t->gain  ), t->wet), qmul(process[i], t->dry));
		//process[i] = qadd(qmul(qmul( s, t->gain ), t->wet), qmul(process[i], t->dry));
		//process[i] = qadd(qmul(s, t->wet), qmul(process[i], t->dry));
		s = 0;
		// increment write pointer
		t->wp = (t->wp < t->dsize ? t->wp + 1 : 0);
	}

	return;
}

void PShift_setupPitchParams(PShift *this) {	
	// Setup pitch shift
	this->dsize = 2160;
	this->qdsize = int2q(this->dsize);
	this->rp = 0;
	this->gain = float2q(0.9f);
	this->fdb = 0;
	this->wp = 0;
	
	this->wet = Q1;
	this->dry = 0;

	this->cent = 0;
	this->pitch = 0;
	memset(&pitch_delay, 0, sizeof(fixedp)*2160);
}

void PShift_setPitchStep(PShift *this, short step) {
	this->pitch = step;	
}

void pitcht_setParam(PShift *t, Uint32 param, int val) {
	switch(param) {
	case PITCHT_CENT:
		t->cent = val;
		break;
	case PITCHT_FEEDBACK:
		t->fdb = val;
		break;
	case PITCHT_PITCHSTEP:
		PShift_setPitchStep(t, val);
		break;
	case PITCHT_MIX:
		t->wet = val;
		if(t->wet > Q1) { t->wet = Q1; }
		t->dry = Q1 - t->wet;
		break;
	case PITCHT_GAIN:
		t->gain = val*4;
		break;
	}
}
