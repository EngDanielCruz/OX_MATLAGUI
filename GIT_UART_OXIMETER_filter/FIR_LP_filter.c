/******************************* SOURCE LICENSE *********************************
Copyright (c) 2015 MicroModeler.

A non-exclusive, nontransferable, perpetual, royalty-free license is granted to the Licensee to
use the following Information for academic, non-profit, or government-sponsored research purposes.
Use of the following Information under this License is restricted to NON-COMMERCIAL PURPOSES ONLY.
Commercial use of the following Information requires a separately executed written license agreement.

This Information is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

******************************* END OF LICENSE *********************************/

// A commercial license for MicroModeler DSP can be obtained at http://www.micromodeler.com/launch.jsp

#include "FIR_LP_filter.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

#ifdef LP_FILTER_50sps
float FIR_LP_filter_coefficients[69] =
{
        -0.00003625556,-0.00012285444,-0.00025883471,-0.00048277213,-0.00076415157,-0.00108414632,-0.00136353681,-0.00151307404,-0.00141246896,-0.00095875340,
        -0.00008171807,0.00120815076,0.00279865577,0.00445562694,0.00583834946,0.00653756503,0.00614898047,0.00436478248,0.00107485824,-0.00354742282,-0.00899371784,
        -0.01442470122,-0.01873906329,-0.02070786245,-0.01915924624,-0.01318708435,-0.00234764582,0.01319342386,0.03259700537,0.05438983440,0.07661074400,0.09704735875,
        0.11352951080,0.12423283610,0.12794262170,0.12423283610,0.11352951080,0.09704735875,0.07661074400,0.05438983440,0.03259700537,0.01319342386,-0.00234764582,
        -0.01318708435,-0.01915924624,-0.02070786245,-0.01873906329,-0.01442470122,-0.00899371784,-0.00354742282,0.00107485824,0.00436478248,0.00614898047,0.00653756503,
        0.00583834946,0.00445562694,0.00279865577,0.00120815076,-0.00008171807,-0.00095875340,-0.00141246896,-0.00151307404,-0.00136353681,-0.00108414632,-0.00076415157,
        -0.00048277213,-0.00025883471,-0.00012285444,-0.00003625556
};
#endif

#ifdef LP_FILTER_100sps
float FIR_LP_filter_coefficients[69] =
{
        -0.00003778299,0.00005609166,0.00014785767,0.00030849027,0.00054235343,0.00084432511,0.00119304645,0.00154695986,0.00184334652,
        0.00200103573,0.00192763796,0.00153115613,0.00073546357,-0.00050203205,-0.00217240094,-0.00419986155,-0.00643186597,-0.00863707159,
        -0.01051353756,-0.01170748100,-0.01184256002,-0.01055719610,-0.00754677272,-0.00260593183,0.00433411729,0.01317837741,0.02365566790,
        0.03532319143,0.04759045318,0.05975955352,0.07108052820,0.08081494272,0.08830162138,0.09301774949,0.09462825209,0.09301774949,0.08830162138,
        0.08081494272,0.07108052820,0.05975955352,0.04759045318,0.03532319143,0.02365566790,0.01317837741,0.00433411729,-0.00260593183,-0.00754677272,
        -0.01055719610,-0.01184256002,-0.01170748100,-0.01051353756,-0.00863707159,-0.00643186597,-0.00419986155,-0.00217240094,-0.00050203205,
        0.00073546357,0.00153115613,0.00192763796,0.00200103573,0.00184334652,0.00154695986,0.00119304645,0.00084432511,0.00054235343,0.00030849027,
        0.00014785767,0.00005609166,-0.00003778299
};
#endif

#ifdef LP_FILTER_200sps
float FIR_LP_filter_coefficients[137] =
{
        -0.00004425356,0.00001672909,0.00002776391,0.00004677810,0.00007394301,0.00010966558,0.00015437789,0.00020830464,0.00027138775,0.00034309391,
        0.00042241922,0.00050774054,0.00059679546,0.00068664958,0.00077369879,0.00085367903,0.00092174165,0.00097253313,0.00100034312,0.00099924300,
        0.00096332270,0.00088687852,0.00076471292,0.00059237616,0.00036648611,0.00008497982,-0.00025257375,-0.00064473844,-0.00108796102,-0.00157639373,
        -0.00210174709,-0.00265323767,-0.00321757537,-0.00377906347,-0.00431975303,-0.00481971772,-0.00525734527,-0.00560980057,-0.00585350068,-0.00596468803,
        -0.00592007767,-0.00569745898,-0.00527648255,-0.00463901879,-0.00377036375,-0.00265941583,-0.00129931816,0.00031186780,0.00217101164,0.00426946254,
        0.00659308163,0.00912205223,0.01183121651,0.01469025295,0.01766413264,0.02071373351,0.02379654348,0.02686745673,0.02987970784,0.03278583661,0.03553876281,
        0.03809276968,0.04040461779,0.04243446514,0.04414686933,0.04551156238,0.04650424048,0.04710708186,0.04730924964,0.04710708186,0.04650424048,0.04551156238,
        0.04414686933,0.04243446514,0.04040461779,0.03809276968,0.03553876281,0.03278583661,0.02987970784,0.02686745673,0.02379654348,0.02071373351,0.01766413264,
        0.01469025295,0.01183121651,0.00912205223,0.00659308163,0.00426946254,0.00217101164,0.00031186780,-0.00129931816,-0.00265941583,-0.00377036375,-0.00463901879,
        -0.00527648255,-0.00569745898,-0.00592007767,-0.00596468803,-0.00585350068,-0.00560980057,-0.00525734527,-0.00481971772,-0.00431975303,-0.00377906347,-0.00321757537,
        -0.00265323767,-0.00210174709,-0.00157639373,-0.00108796102,-0.00064473844,-0.00025257375,0.00008497982,0.00036648611,0.00059237616,0.00076471292,0.00088687852,
        0.00096332270,0.00099924300,0.00100034312,0.00097253313,0.00092174165,0.00085367903,0.00077369879,0.00068664958,0.00059679546,0.00050774054,0.00042241922,
        0.00034309391,0.00027138775,0.00020830464,0.00015437789,0.00010966558,0.00007394301,0.00004677810,0.00002776391,0.00001672909,-0.00004425356
};
#endif

#ifdef LP_FILTER_400sps
float FIR_LP_filter_coefficients[273] =
{-0.00004760576,0.00000695349,0.00000819606,0.00001047028,0.00001374642,0.00001800377,0.00002325982,0.00002952459,0.00003683507,0.00004521047,
        0.00005469044,0.00006529024,0.00007703964,0.00008993937,0.00010400080,0.00011920345,0.00013553438,0.00015294523,0.00017139169,0.00019078820,
        0.00021105814,0.00023207755,0.00025372978,0.00027584811,0.00029827619,0.00032080314,0.00034322933,0.00036530488,0.00038678609,0.00040738276,
        0.00042681440,0.00044475484,0.00046089230,0.00047486869,0.00048634785,0.00049494737,0.00050031836,0.00050206418,0.00049983780,0.00049323624,
        0.00048193938,0.00046555870,0.00044372867,0.00041629039,0.00038277108,0.00034297231,0.00029665293,0.00024363771,0.00018376032,0.00011692067,
        0.00004305573,-0.00003783190,-0.00012568697,-0.00022038707,-0.00032174677,-0.00042951023,-0.00054334995,-0.00066286378,-0.00078757218,-0.00091691921,
        -0.00105026935,-0.00118691369,-0.00132605562,-0.00146682886,-0.00160828384,-0.00174941006,-0.00188910740,-0.00202623080,-0.00215955149,-0.00228780019,
        -0.00240964140,-0.00252370723,-0.00262857694,-0.00272281142,-0.00280493428,-0.00287347054,-0.00292692240,-0.00296381512,-0.00298266648,-0.00298204087,
        -0.00296051148,-0.00291672815,-0.00284935441,-0.00275716558,-0.00263898959,-0.00249371305,-0.00232039159,-0.00211813534,-0.00188618596,-0.00162390294,
        -0.00133079314,-0.00100650010,-0.00065082085,-0.00026370594,0.00015473139,0.00060421089,0.00108428206,0.00159431971,0.00213352568,0.00270092487,
        0.00329536921,0.00391553715,0.00455993554,0.00522690220,0.00591461919,0.00662111398,0.00734426640,0.00808180776,0.00883136224,0.00959040783,
        0.01035634149,0.01112644747,0.01189794112,0.01266796049,0.01343360171,0.01419191528,0.01493993867,0.01567469351,0.01639322191,0.01709258370,
        0.01776989549,0.01842231862,0.01904710755,0.01964158565,0.02020321786,0.02072955482,0.02121830732,0.02166734077,0.02207465842,0.02243845724,
        .02275712043,0.02302922308,0.02325354517,0.02342907526,0.02355502360,0.02363082021,0.02365612425,0.02363082021,0.02355502360,0.02342907526,
        0.02325354517,0.02302922308,0.02275712043,0.02243845724,0.02207465842,0.02166734077,0.02121830732,0.02072955482,0.02020321786,0.01964158565,
        .01904710755,0.01842231862,0.01776989549,0.01709258370,0.01639322191,0.01567469351,0.01493993867,0.01419191528,0.01343360171,0.01266796049,
        0.01189794112,0.01112644747,0.01035634149,0.00959040783,0.00883136224,0.00808180776,0.00734426640,0.00662111398,0.00591461919,0.00522690220,
        0.00455993554,0.00391553715,0.00329536921,0.00270092487,0.00213352568,0.00159431971,0.00108428206,0.00060421089,0.00015473139,-0.00026370594,
        -0.00065082085,-0.00100650010,-0.00133079314,-0.00162390294,-0.00188618596,-0.00211813534,-0.00232039159,-0.00249371305,-0.00263898959,-0.00275716558,
        -0.00284935441,-0.00291672815,-0.00296051148,-0.00298204087,-0.00298266648,-0.00296381512,-0.00292692240,-0.00287347054,-0.00280493428,-0.00272281142,
        -0.00262857694,-0.00252370723,-0.00240964140,-0.00228780019,-0.00215955149,-0.00202623080,-0.00188910740,-0.00174941006,-0.00160828384,-0.00146682886,
        -0.00132605562,-0.00118691369,-0.00105026935,-0.00091691921,-0.00078757218,-0.00066286378,-0.00054334995,-0.00042951023,-0.00032174677,-0.00022038707,
        -0.00012568697,-0.00003783190,0.00004305573,0.00011692067,0.00018376032,0.00024363771,0.00029665293,0.00034297231,0.00038277108,0.00041629039,
        0.00044372867,0.00046555870,0.00048193938,0.00049323624,0.00049983780,0.00050206418,0.00050031836,0.00049494737,0.00048634785,0.00047486869,
        0.00046089230,0.00044475484,0.00042681440,0.00040738276,0.00038678609,0.00036530488,0.00034322933,0.00032080314,0.00029827619,0.00027584811,
        0.00025372978,0.00023207755,0.00021105814,0.00019078820,0.00017139169,0.00015294523,0.00013553438,0.00011920345,0.00010400080,0.00008993937,
        0.00007703964,0.00006529024,0.00005469044,0.00004521047,0.00003683507,0.00002952459,0.00002325982,0.00001800377,0.00001374642,0.00001047028,
        0.00000819606,0.00000695349,-0.00004760576

};
#endif



#ifdef LP_FILTER_1000sps
float FIR_LP_filter_coefficients[681] ={
        0.00002267396,0.00000486683,0.00000538437,0.00000593276,0.00000651456,0.00000712856,0.00000777714,0.00000845904,0.00000917693,0.00000992962,0.00001072010,0.00001154730,
        0.00001241416,0.00001331935,0.00001426549,0.00001525061,0.00001627708,0.00001734251,0.00001844940,0.00001959566,0.00002078444,0.00002201377,0.00002328712,0.00002460164,
        0.00002595976,0.00002735686,0.00002879489,0.00003026907,0.00003178380,0.00003333620,0.00003493279,0.00003656768,0.00003824264,0.00003994416,0.00004167833,0.00004344265,
        0.00004526232,0.00004710011,0.00004894573,0.00005085415,0.00005276130,0.00005470555,0.00005666269,0.00005864367,0.00006063644,0.00006264642,0.00006466432,0.00006669327,
        0.00006872486,0.00007076054,0.00007279252,0.00007482125,0.00007683961,0.00007884768,0.00008083854,0.00008281154,0.00008475925,0.00008667982,0.00008856560,0.00009041483,
        0.00009222051,0.00009398117,0.00009568944,0.00009734234,0.00009893096,0.00010045221,0.00010189853,0.00010326874,0.00010455438,0.00010575108,0.00010684783,0.00010784417,
        0.00010873346,0.00010951239,0.00011016158,0.00011069115,0.00011108724,0.00011133950,0.00011145102,0.00011140649,0.00011120562,0.00011083749,0.00011029828,0.00010957915,
        0.00010867539,0.00010757901,0.00010628503,0.00010478568,0.00010307536,0.00010114653,0.00009899345,0.00009660923,0.00009398872,0.00009112533,0.00008801363,0.00008464692,
        0.00008101975,0.00007712604,0.00007296136,0.00006852017,0.00006379771,0.00005878799,0.00005348676,0.00004788974,0.00004199398,0.00003579425,0.00002928644,0.00002246649,
        0.00001533380,0.00000788448,0.00000011472,-0.00000797796,-0.00001639003,-0.00002513140,-0.00003419672,-0.00004358924,-0.00005330994,-0.00006335658,-0.00007373071,-0.00008442970,
        -0.00009545362,-0.00010679982,-0.00011846683,-0.00013045102,-0.00014274982,-0.00015535869,-0.00016827422,-0.00018149121,-0.00019500489,-0.00020880888,-0.00022289720,-0.00023726249,
        -0.00025189796,-0.00026679543,-0.00028194650,-0.00029734129,-0.00031297037,-0.00032882334,-0.00034489011,-0.00036115863,-0.00037761664,-0.00039425102,-0.00041104938,-0.00042799758,
        -0.00044508078,-0.00046228323,-0.00047959085,-0.00049698696,-0.00051445334,-0.00053197303,-0.00054953003,-0.00056710123,-0.00058467226,-0.00060221989,-0.00061972550,-0.00063716742,
        -0.00065452431,-0.00067177415,-0.00068889448,-0.00070586213,-0.00072265370,-0.00073924480,-0.00075561105,-0.00077172747,-0.00078756880,-0.00080310908,-0.00081832201,-0.00083318056,
        -0.00084765785,-0.00086172647,-0.00087535911,-0.00088852720,-0.00090120238,-0.00091335591,-0.00092495914,-0.00093598285,-0.00094639766,-0.00095617358,-0.00096528162,-0.00097369193,
        -0.00098137441,-0.00098829845,-0.00099443481,-0.00099975336,-0.00100422383,-0.00100781559,-0.00101050048,-0.00101224671,-0.00101302529,-0.00101280771,-0.00101156265,-0.00100926298,
        -0.00100587832,-0.00100138085,-0.00099574216,-0.00098893431,-0.00098093005,-0.00097170222,-0.00096122391,-0.00094946910,-0.00093641219,-0.00092202809,-0.00090629252,-0.00088918157,
        -0.00087067165,-0.00085074047,-0.00082936633,-0.00080652803,-0.00078220514,-0.00075637823,-0.00072902878,-0.00070013909,-0.00066969218,-0.00063767191,-0.00060406351,-0.00056885317,
        -0.00053202774,-0.00049357512,-0.00045348456,-0.00041174673,-0.00036835257,-0.00032329449,-0.00027656669,-0.00022816405,-0.00017808152,-0.00012631803,-0.00007287113,-0.00001774079,
        0.00003907090,0.00009756301,0.00015773032,0.00021956894,0.00028307157,0.00034823074,0.00041503695,0.00048348002,0.00055354758,0.00062522665,0.00069850206,0.00077335793,0.00084977644,
        0.00092773896,0.00100722455,0.00108821178,0.00117067702,0.00125459617,0.00133994268,0.00142668944,0.00151480723,0.00160426635,0.00169503491,0.00178708008,0.00188036729,0.00197486160,
        0.00207052543,0.00216732104,0.00226520863,0.00236414792,0.00246409676,0.00256501162,0.00266684871,0.00276956242,0.00287310570,0.00297743199,0.00308249099,0.00318823406,0.00329460995,
        0.00340156653,0.00350905210,0.00361701194,0.00372539228,0.00383413746,0.00394319184,0.00405249791,0.00416199910,0.00427163625,0.00438135117,0.00449108426,0.00460077589,0.00471036509,
        0.00481979176,0.00492899446,0.00503791124,0.00514648110,0.00525464118,0.00536232954,0.00546948379,0.00557604153,0.00568194035,0.00578711741,0.00589151029,0.00599505706,0.00609769533,
        0.00619936269,0.00629999861,0.00639954023,0.00649792794,0.00659510074,0.00669099810,0.00678556087,0.00687873038,0.00697044749,0.00706065586,0.00714929681,0.00723631587,0.00732165715,
        0.00740526570,0.00748708891,0.00756707368,0.00764516927,0.00772132399,0.00779548986,0.00786761753,0.00793766137,0.00800557528,0.00807131361,0.00813483447,0.00819609687,0.00825505890,
        0.00831168238,0.00836592913,0.00841776468,0.00846715458,0.00851406436,0.00855846424,0.00860032346,0.00863961596,0.00867631380,0.00871039275,0.00874183048,0.00877060648,0.00879669935,
        0.00882009324,0.00884077326,0.00885872264,0.00887393206,0.00888638943,0.00889608823,0.00890301913,0.00890718121,0.00890856795,0.00890718121,0.00890301913,0.00889608823,0.00888638943,
        0.00887393206,0.00885872264,0.00884077326,0.00882009324,0.00879669935,0.00877060648,0.00874183048,0.00871039275,0.00867631380,0.00863961596,0.00860032346,0.00855846424,0.00851406436,
        0.00846715458,0.00841776468,0.00836592913,0.00831168238,0.00825505890,0.00819609687,0.00813483447,0.00807131361,0.00800557528,0.00793766137,0.00786761753,0.00779548986,0.00772132399,
        0.00764516927,0.00756707368,0.00748708891,0.00740526570,0.00732165715,0.00723631587,0.00714929681,0.00706065586,0.00697044749,0.00687873038,0.00678556087,0.00669099810,0.00659510074,
        0.00649792794,0.00639954023,0.00629999861,0.00619936269,0.00609769533,0.00599505706,0.00589151029,0.00578711741,0.00568194035,0.00557604153,0.00546948379,0.00536232954,0.00525464118,
        0.00514648110,0.00503791124,0.00492899446,0.00481979176,0.00471036509,0.00460077589,0.00449108426,0.00438135117,0.00427163625,0.00416199910,0.00405249791,0.00394319184,0.00383413746,
        0.00372539228,0.00361701194,0.00350905210,0.00340156653,0.00329460995,0.00318823406,0.00308249099,0.00297743199,0.00287310570,0.00276956242,0.00266684871,0.00256501162,0.00246409676,
        0.00236414792,0.00226520863,0.00216732104,0.00207052543,0.00197486160,0.00188036729,0.00178708008,0.00169503491,0.00160426635,0.00151480723,0.00142668944,0.00133994268,0.00125459617,
        0.00117067702,0.00108821178,0.00100722455,0.00092773896,0.00084977644,0.00077335793,0.00069850206,0.00062522665,0.00055354758,0.00048348002,0.00041503695,0.00034823074,0.00028307157,
        0.00021956894,0.00015773032,0.00009756301,0.00003907090,-0.00001774079,-0.00007287113,-0.00012631803,-0.00017808152,-0.00022816405,-0.00027656669,-0.00032329449,-0.00036835257,-0.00041174673,
        -0.00045348456,-0.00049357512,-0.00053202774,-0.00056885317,-0.00060406351,-0.00063767191,-0.00066969218,-0.00070013909,-0.00072902878,-0.00075637823,-0.00078220514,-0.00080652803,
        -0.00082936633,-0.00085074047,-0.00087067165,-0.00088918157,-0.00090629252,-0.00092202809,-0.00093641219,-0.00094946910,-0.00096122391,-0.00097170222,-0.00098093005,-0.00098893431,
        -0.00099574216,-0.00100138085,-0.00100587832,-0.00100926298,-0.00101156265,-0.00101280771,-0.00101302529,-0.00101224671,-0.00101050048,-0.00100781559,-0.00100422383,-0.00099975336,
        -0.00099443481,-0.00098829845,-0.00098137441,-0.00097369193,-0.00096528162,-0.00095617358,-0.00094639766,-0.00093598285,-0.00092495914,-0.00091335591,-0.00090120238,-0.00088852720,
        -0.00087535911,-0.00086172647,-0.00084765785,-0.00083318056,-0.00081832201,-0.00080310908,-0.00078756880,-0.00077172747,-0.00075561105,-0.00073924480,-0.00072265370,-0.00070586213,
        -0.00068889448,-0.00067177415,-0.00065452431,-0.00063716742,-0.00061972550,-0.00060221989,-0.00058467226,-0.00056710123,-0.00054953003,-0.00053197303,-0.00051445334,-0.00049698696,
        -0.00047959085,-0.00046228323,-0.00044508078,-0.00042799758,-0.00041104938,-0.00039425102,-0.00037761664,-0.00036115863,-0.00034489011,-0.00032882334,-0.00031297037,-0.00029734129,
        -0.00028194650,-0.00026679543,-0.00025189796,-0.00023726249,-0.00022289720,-0.00020880888,-0.00019500489,-0.00018149121,-0.00016827422,-0.00015535869,-0.00014274982,-0.00013045102,
        -0.00011846683,-0.00010679982,-0.00009545362,-0.00008442970,-0.00007373071,-0.00006335658,-0.00005330994,-0.00004358924,-0.00003419672,-0.00002513140,-0.00001639003,-0.00000797796,
        0.00000011472,0.00000788448,0.00001533380,0.00002246649,0.00002928644,0.00003579425,0.00004199398,0.00004788974,0.00005348676,0.00005878799,0.00006379771,0.00006852017,0.00007296136,
        0.00007712604,0.00008101975,0.00008464692,0.00008801363,0.00009112533,0.00009398872,0.00009660923,0.00009899345,0.00010114653,0.00010307536,0.00010478568,0.00010628503,0.00010757901,
        0.00010867539,0.00010957915,0.00011029828,0.00011083749,0.00011120562,0.00011140649,0.00011145102,0.00011133950,0.00011108724,0.00011069115,0.00011016158,0.00010951239,0.00010873346,
        0.00010784417,0.00010684783,0.00010575108,0.00010455438,0.00010326874,0.00010189853,0.00010045221,0.00009893096,0.00009734234,0.00009568944,0.00009398117,0.00009222051,0.00009041483,
        0.00008856560,0.00008667982,0.00008475925,0.00008281154,0.00008083854,0.00007884768,0.00007683961,0.00007482125,0.00007279252,0.00007076054,0.00006872486,0.00006669327,0.00006466432,
        0.00006264642,0.00006063644,0.00005864367,0.00005666269,0.00005470555,0.00005276130,0.00005085415,0.00004894573,0.00004710011,0.00004526232,0.00004344265,0.00004167833,0.00003994416,
        0.00003824264,0.00003656768,0.00003493279,0.00003333620,0.00003178380,0.00003026907,0.00002879489,0.00002735686,0.00002595976,0.00002460164,0.00002328712,0.00002201377,0.00002078444,
        0.00001959566,0.00001844940,0.00001734251,0.00001627708,0.00001525061,0.00001426549,0.00001331935,0.00001241416,0.00001154730,0.00001072010,0.00000992962,0.00000917693,0.00000845904,
        0.00000777714,0.00000712856,0.00000651456,0.00000593276,0.00000538437,0.00000486683,0.00002267396

};
#endif
FIR_LP_filterType *FIR_LP_filter_create( void )
{
    FIR_LP_filterType *result = (FIR_LP_filterType *)malloc( sizeof( FIR_LP_filterType ) ); // Allocate memory for the object
    FIR_LP_filter_init( result );                                           // Initialize it
    return result;                                                              // Return the result
}

void FIR_LP_filter_destroy( FIR_LP_filterType *pObject )
{
    free( pObject );
}

 void FIR_LP_filter_init( FIR_LP_filterType * pThis )
{
    FIR_LP_filter_reset( pThis );

}

 void FIR_LP_filter_reset( FIR_LP_filterType * pThis )
{
    memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
    pThis->pointer = pThis->state;                      // History buffer points to start of state buffer
    pThis->output = 0;                                  // Reset output

}

 int FIR_LP_filter_filterBlock( FIR_LP_filterType * pThis, float * pInput, float * pOutput, unsigned int count )
{
    float *pOriginalOutput = pOutput;   // Save original output so we can track the number of samples processed
    float accumulator;

    for( ;count; --count )
    {
        pThis->pointer[FIR_LP_filter_length] = *pInput;                     // Copy sample to top of history buffer
        *(pThis->pointer++) = *(pInput++);                                      // Copy sample to bottom of history buffer

        if( pThis->pointer >= pThis->state + FIR_LP_filter_length )             // Handle wrap-around
            pThis->pointer -= FIR_LP_filter_length;

        accumulator = 0;
        FIR_LP_filter_dotProduct( pThis->pointer, FIR_LP_filter_coefficients, &accumulator, FIR_LP_filter_length );
        *(pOutput++) = accumulator; // Store the result
    }

    return pOutput - pOriginalOutput;

}

 void FIR_LP_filter_dotProduct( float * pInput, float * pKernel, float * pAccumulator, short count )
{
    float accumulator = *pAccumulator;
    while( count-- )
        accumulator += ((float)*(pKernel++)) * *(pInput++);
    *pAccumulator = accumulator;

}

