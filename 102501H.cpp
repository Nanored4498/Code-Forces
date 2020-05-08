#include <iostream>

using namespace std;

const long long M = 1LL << 40;
const long long S0 = 0x600DCAFE;

long long f(long long s) {
	return (s + (s >> 20) + 12345) % M;
}

long long period = 182129209LL, start = 350125310LL, block = 546387LL;
long long beg = 175147925LL, loop = 91029304LL, sl = 516914LL;
long long b0[640] = {274505LL, 276164LL, 266699LL, 272380LL, 275832LL, 270994LL, 274873LL, 272912LL, 275089LL, 275742LL, 271362LL, 273071LL, 269884LL, 273759LL, 274052LL, 274930LL, 271953LL, 276092LL, 275913LL, 278636LL, 273473LL, 273975LL, 272642LL, 273379LL, 273201LL, 271789LL, 265609LL, 271123LL, 275886LL, 270895LL, 271701LL, 273501LL, 273926LL, 271830LL, 280264LL, 273932LL, 271572LL, 272818LL, 272245LL, 274479LL, 274053LL, 272810LL, 272138LL, 272165LL, 273203LL, 274776LL, 273096LL, 274639LL, 271944LL, 273221LL, 273057LL, 270298LL, 279930LL, 274818LL, 274154LL, 274677LL, 275141LL, 272918LL, 273796LL, 272170LL, 274312LL, 271674LL, 274320LL, 275462LL, 273838LL, 272919LL, 273126LL, 272513LL, 272672LL, 274705LL, 271035LL, 273198LL, 268763LL, 273520LL, 274994LL, 273758LL, 274600LL, 276807LL, 278478LL, 277081LL, 272490LL, 275572LL, 272998LL, 273778LL, 274670LL, 278825LL, 270356LL, 272243LL, 269385LL, 274436LL, 271542LL, 275060LL, 275186LL, 274604LL, 274192LL, 276509LL, 274567LL, 271210LL, 273633LL, 272619LL, 274828LL, 272971LL, 272567LL, 273091LL, 276997LL, 272579LL, 270539LL, 273269LL, 273994LL, 272708LL, 275909LL, 270578LL, 271308LL, 273153LL, 274024LL, 272799LL, 272074LL, 273631LL, 273737LL, 272409LL, 277600LL, 276052LL, 270736LL, 273311LL, 273310LL, 273553LL, 275447LL, 272648LL, 272869LL, 274559LL, 269239LL, 269304LL, 273792LL, 275789LL, 273227LL, 273470LL, 272806LL, 273710LL, 267926LL, 273553LL, 274609LL, 275017LL, 272928LL, 272508LL, 273295LL, 274754LL, 275842LL, 277478LL, 272147LL, 269576LL, 275543LL, 273712LL, 274170LL, 273574LL, 274179LL, 275104LL, 270596LL, 276315LL, 273465LL, 272760LL, 275537LL, 272996LL, 277274LL, 269663LL, 275624LL, 272804LL, 271291LL, 272808LL, 272592LL, 272958LL, 275321LL, 269712LL, 272698LL, 270919LL, 273959LL, 270786LL, 272647LL, 272771LL, 274150LL, 274596LL, 274427LL, 276021LL, 277725LL, 274617LL, 272973LL, 273178LL, 274958LL, 272922LL, 271100LL, 274448LL, 276876LL, 277483LL, 272244LL, 272331LL, 273355LL, 273574LL, 273414LL, 266433LL, 270859LL, 271591LL, 274627LL, 273636LL, 274130LL, 272270LL, 271485LL, 272337LL, 275624LL, 276542LL, 274300LL, 273374LL, 272959LL, 273539LL, 274482LL, 272724LL, 273329LL, 271319LL, 274113LL, 274903LL, 271328LL, 270013LL, 274833LL, 273052LL, 274920LL, 272254LL, 271708LL, 272508LL, 271822LL, 271640LL, 272302LL, 273077LL, 274983LL, 278302LL, 271079LL, 272361LL, 274658LL, 274312LL, 275003LL, 274006LL, 273261LL, 274612LL, 275291LL, 275284LL, 277001LL, 273309LL, 273858LL, 274692LL, 273607LL, 271276LL, 277702LL, 271864LL, 274676LL, 271294LL, 274151LL, 275091LL, 272605LL, 273676LL, 271177LL, 269311LL, 273480LL, 272999LL, 273395LL, 270986LL, 272611LL, 273825LL, 275207LL, 271507LL, 274186LL, 276213LL, 274681LL, 272554LL, 273546LL, 271930LL, 272998LL, 274602LL, 273404LL, 274545LL, 276711LL, 272509LL, 272990LL, 273774LL, 273368LL, 276453LL, 274245LL, 272564LL, 273532LL, 271849LL, 276684LL, 274579LL, 272224LL, 274185LL, 271912LL, 275452LL, 272031LL, 273921LL, 273939LL, 271619LL, 271378LL, 271946LL, 271775LL, 272216LL, 270934LL, 270167LL, 270988LL, 273574LL, 271525LL, 274061LL, 272830LL, 273730LL, 275005LL, 274991LL, 271372LL, 272709LL, 274153LL, 274414LL, 271932LL, 272599LL, 272460LL, 267903LL, 273265LL, 273709LL, 271163LL, 270393LL, 273065LL, 271927LL, 273454LL, 268612LL, 271689LL, 271701LL, 274410LL, 274009LL, 272675LL, 270909LL, 273160LL, 275110LL, 269114LL, 275390LL, 275474LL, 271853LL, 270564LL, 272205LL, 273052LL, 275703LL, 277984LL, 277620LL, 275087LL, 273430LL, 273262LL, 272221LL, 274164LL, 273425LL, 274477LL, 275946LL, 278625LL, 277112LL, 273681LL, 273414LL, 273205LL, 274409LL, 274592LL, 274092LL, 269628LL, 277417LL, 275495LL, 272126LL, 273890LL, 273239LL, 273530LL, 272889LL, 267034LL, 269584LL, 277197LL, 269201LL, 272402LL, 272602LL, 273685LL, 271476LL, 281403LL, 270477LL, 271967LL, 273311LL, 270756LL, 273844LL, 273301LL, 274830LL, 268292LL, 273077LL, 274958LL, 271276LL, 276261LL, 273085LL, 274259LL, 269515LL, 273124LL, 278041LL, 276504LL, 278842LL, 274517LL, 273240LL, 270062LL, 272765LL, 275985LL, 271362LL, 264975LL, 277214LL, 275477LL, 272407LL, 270317LL, 272958LL, 272994LL, 272957LL, 272378LL, 270545LL, 270389LL, 273923LL, 274963LL, 273683LL, 273549LL, 272509LL, 271603LL, 271632LL, 268557LL, 273178LL, 272489LL, 273992LL, 272287LL, 274447LL, 273835LL, 276752LL, 277968LL, 278854LL, 274464LL, 273120LL, 273265LL, 274029LL, 274587LL, 276339LL, 272844LL, 273554LL, 270904LL, 273409LL, 272730LL, 274975LL, 271532LL, 273795LL, 277149LL, 274697LL, 275529LL, 276722LL, 275276LL, 272934LL, 275109LL, 271923LL, 272553LL, 268475LL, 278490LL, 271811LL, 269944LL, 274007LL, 272351LL, 273870LL, 271019LL, 276734LL, 272541LL, 270281LL, 272420LL, 274613LL, 271852LL, 273244LL, 275134LL, 270734LL, 275814LL, 273174LL, 278021LL, 274924LL, 270835LL, 274275LL, 274212LL, 275760LL, 268725LL, 269186LL, 272250LL, 276016LL, 274450LL, 275800LL, 273976LL, 272140LL, 273428LL, 276078LL, 280231LL, 267534LL, 274436LL, 274211LL, 274034LL, 274320LL, 272527LL, 271055LL, 273254LL, 268726LL, 271432LL, 273734LL, 271818LL, 274492LL, 272224LL, 273475LL, 272066LL, 277237LL, 277868LL, 277105LL, 271248LL, 270505LL, 272281LL, 273112LL, 278317LL, 273141LL, 272518LL, 272413LL, 275849LL, 272289LL, 272988LL, 273854LL, 273130LL, 266411LL, 270509LL, 276294LL, 276848LL, 274222LL, 272684LL, 272341LL, 273967LL, 273944LL, 277363LL, 271595LL, 271269LL, 271923LL, 275296LL, 272138LL, 271340LL, 274062LL, 273111LL, 275742LL, 275739LL, 271592LL, 272731LL, 274264LL, 275327LL, 272263LL, 270707LL, 278593LL, 269697LL, 274969LL, 272739LL, 273524LL, 274526LL, 271794LL, 271788LL, 268644LL, 271741LL, 273981LL, 276117LL, 273309LL, 272762LL, 273309LL, 274760LL, 274077LL, 272731LL, 271122LL, 274860LL, 270345LL, 271989LL, 275164LL, 272946LL, 271854LL, 272095LL, 271454LL, 270371LL, 273050LL, 272339LL, 275118LL, 272275LL, 271481LL, 274326LL, 274269LL, 270951LL, 273477LL, 273823LL, 271683LL, 270251LL, 274882LL, 272934LL, 271041LL, 281086LL, 271196LL, 271858LL, 271936LL, 272342LL, 272487LL, 273630LL, 273363LL, 275206LL, 275288LL, 271854LL, 275311LL, 272193LL, 272011LL, 273708LL, 271271LL, 271696LL, 270534LL, 273436LL, 273641LL, 273750LL, 274058LL, 273771LL, 271930LL, 273876LL, 275165LL, 270994LL, 269017LL, 273161LL, 275321LL, 272498LL, 272892LL, 273892LL, 273480LL, 272729LL, 270012LL, 274108LL, 274337LL, 272286LL, 273706LL, 272594LL, 272717LL, 264975LL, 272977LL, 272574LL, 270100LL, 272014LL, 271566LL, 272760LL, 274805LL};
long long s0[640] = {11565191487LL, 28325578800LL, 56547335774LL, 104068168704LL, 184085496085LL, 318821616714LL, 545695255176LL, 927713437252LL, 5486335147LL, 18089776771LL, 39311915585LL, 75046533999LL, 135217799524LL, 236536390896LL, 407140363588LL, 694409611527LL, 915247421LL, 10392811018LL, 26351481422LL, 53223277909LL, 98470997846LL, 174660773725LL, 302951922369LL, 518973279874LL, 882717991308LL, 4604806841LL, 16605426823LL, 36812512494LL, 70837946975LL, 128131225934LL, 224603757311LL, 387047756513LL, 660576940298LL, 251912615LL, 9275863423LL, 24470723888LL, 50056389142LL, 93138473863LL, 165681673822LL, 287832583503LL, 493514780566LL, 839850033649LL, 3765599664LL, 15192339047LL, 34433103268LL, 66831410282LL, 121384871824LL, 213243998180LL, 367919775547LL, 628368541754LL, 1066921759399LL, 8214126924LL, 22682932969LL, 47046041016LL, 88069539010LL, 157146414613LL, 273460599912LL, 469314705793LL, 799101056842LL, 2966341933LL, 13846519341LL, 32166962540LL, 63015599005LL, 114959668479LL, 202425004215LL, 349702355579LL, 597693379910LL, 1015269793233LL, 7201232918LL, 20977384785LL, 44174176213LL, 83233787370LL, 149003797993LL, 259749762394LL, 446227892838LL, 760226631513LL, 2204639219LL, 12563936288LL, 30007302001LL, 59379083138LL, 108836369073LL, 192114367305LL, 332340925565LL, 568459565105LL, 966044819771LL, 6237970133LL, 19355407412LL, 41443030935LL, 78634984380LL, 141260164113LL, 246710746757LL, 424272318249LL, 723257027331LL, 1480737288LL, 11345003667LL, 27954818329LL, 55923035680LL, 103016949312LL, 182315414245LL, 315841087758LL, 540676530613LL, 919262723971LL, 5321040690LL, 17811447796LL, 38843255078LL, 74257386011LL, 133889003094LL, 234298914506LL, 403372818558LL, 688065680924LL, 791365751LL, 10184214463LL, 26000238776LL, 52631842580LL, 97475117206LL, 172983873334LL, 300128295780LL, 514218753441LL, 874712145356LL, 4448219415LL, 16341759231LL, 36368539289LL, 70090368565LL, 126872425690LL, 222484143164LL, 383478671682LL, 654567183985LL, 134876823LL, 9078794261LL, 24138891539LL, 49497637586LL, 92197627415LL, 164097441624LL, 285164995000LL, 489022996824LL, 832286603068LL, 3616985936LL, 14942097674LL, 34011737647LL, 66121899182LL, 120190170959LL, 211232316689LL, 364532432347LL, 622664808921LL, 1057317604277LL, 8026643237LL, 22367241044LL, 46514467346LL, 87174455678LL, 155639240452LL, 270922764554LL, 465041405287LL, 791905516091LL, 2825540585LL, 13609432963LL, 31767747775LL, 62343386554LL, 113827772346LL, 200519075939LL, 346493083194LL, 592289488908LL, 1006170522824LL, 7023319938LL, 20677808307LL, 43669738219LL, 82384396064LL, 147573561586LL, 257341477716LL, 442172734296LL, 753398406843LL, 2071348374LL, 12339496261LL, 29629381606LL, 58742726871LL, 107764849054LL, 190310102421LL, 329302837913LL, 563343921394LL, 957430910768LL, 6069154308LL, 19071149065LL, 40964386258LL, 77829024838LL, 139903059935LL, 244425604814LL, 420424512371LL, 716777950159LL, 1353739716LL, 11131160438LL, 27594741277LL, 55316724581LL, 101996020304LL, 180596336413LL, 312946441276LL, 535802418581LL, 911055514706LL, 5160799649LL, 17541627993LL, 38388922477LL, 73492364089LL, 132600831174LL, 232129843101LL, 399720456788LL, 681915699458LL, 669998484LL, 9979851830LL, 25656125500LL, 52052412081LL, 96499450817LL, 171341010505LL, 297361982929LL, 509560734056LL, 866868800612LL, 4293961742LL, 16082014481LL, 35931171559LL, 69353912803LL, 125632354552LL, 220396065761LL, 379962690548LL, 648646845409LL, 18249685LL, 8882413228LL, 23808218035LL, 48940837406LL, 91260066966LL, 162518742978LL, 282506724048LL, 484546902484LL, 824749590146LL, 3469033625LL, 14692970195LL, 33592247644LL, 65415546604LL, 119000788592LL, 209229591007LL, 361160169442LL, 616986468210LL, 1047756205229LL, 7838010401LL, 22049614073LL, 45979635393LL, 86273885801LL, 154122827594LL, 268369372763LL, 460741910017LL, 784665868383LL, 2684128624LL, 13371318183LL, 31366801365LL, 61668258112LL, 112690966147LL, 198604879822LL, 343269889535LL, 586862156945LL, 997031781182LL, 6845307693LL, 20378064663LL, 43165018869LL, 81534531104LL, 146142527870LL, 254931850697LL, 438115315741LL, 746566375858LL, 1937365698LL, 12113891451LL, 29249499970LL, 58103068197LL, 106687768257LL, 188496474164LL, 326248984010LL, 558201729553LL, 948772298845LL, 5898899833LL, 18784468300LL, 40481662738LL, 77016197095LL, 138534390742LL, 242120989146LL, 416543915891LL, 710243659192LL, 1225982476LL, 10916038139LL, 27232510513LL, 54706787122LL, 100968985265LL, 178866976997LL, 310034482111LL, 530899154701LL, 902799218367LL, 4998226191LL, 17267880840LL, 37927977054LL, 72716207196LL, 131293909391LL, 229929200239LL, 396014932766LL, 675676202074LL, 548594920LL, 9775428011LL, 25311909094LL, 51472807738LL, 95523491690LL, 169697654403LL, 294594839413LL, 504901315971LL, 859023101173LL, 4140816935LL, 15824143610LL, 35496959164LL, 68622769789LL, 124401228984LL, 218323051244LL, 376472072055LL, 642769213875LL, 1091170140914LL, 8689779217LL, 23483853712LL, 48394660837LL, 90340394566LL, 160970164916LL, 279899171412LL, 480156209353LL, 817356379849LL, 3323553667LL, 14448005708LL, 33179767372LL, 64720997189LL, 117831280647LL, 207260330661LL, 357844255886LL, 611403011029LL, 1038354574552LL, 7654464438LL, 21740552605LL, 45459226280LL, 85397601883LL, 152647308444LL, 265884839505LL, 456558361525LL, 777621455491LL, 2546022907LL, 13138770844LL, 30975229531LL, 61008915243LL, 111580740358LL, 196735440756LL, 340122059190LL, 581561723947LL, 988106716966LL, 6669859949LL, 20082639333LL, 42667570900LL, 80696909780LL, 144732110072LL, 252556937148LL, 434116348959LL, 739832768831LL, 1805621449LL, 11892055521LL, 28875964468LL, 57474095406LL, 105628680763LL, 186713143733LL, 323246146599LL, 553145441731LL, 940258335397LL, 5731692165LL, 18502917890LL, 40007577725LL, 76217915474LL, 137190214721LL, 239857616373LL, 412732765245LL, 703826303063LL, 1099911541LL, 10703755296LL, 26875060763LL, 54104900128LL, 99955505694LL, 177160442686LL, 307160957132LL, 526060607361LL, 894651894228LL, 4838763120LL, 16999371035LL, 37475850266LL, 71954899371LL, 130011991372LL, 227770659454LL, 392380302554LL, 669556077666LL, 428786456LL, 9573690144LL, 24972215530LL, 50900819205LL, 94560356394LL, 168075891955LL, 291864056054LL, 500303122567LL, 851280493768LL, 3988580101LL, 15567801736LL, 35065321135LL, 67895961956LL, 123177403131LL, 216262328348LL, 373002150856LL, 636926432356LL, 1081331850593LL, 8496498708LL, 23158400880LL, 47846651362LL, 89417636094LL, 159416389996LL, 277282868083LL, 475750781464LL, 809938358625LL, 3179223174LL, 14204976639LL, 32770546219LL, 64031935377LL, 116671012982LL, 205306629122LL, 354554541464LL, 605863669151LL, 1029027226791LL, 7472186456LL, 21433626232LL, 44942412328LL, 84527371571LL, 151181982768LL, 263417470081LL, 452403714000LL, 770625706551LL, 2408705161LL, 12907550046LL, 30585891446LL, 60353333342LL, 110476847523LL, 194876665454LL, 336992184816LL, 576291526421LL, 979232564806LL, 6496133485LL, 19790112324LL, 42175003125LL, 79867506038LL, 143335529604LL, 250205323434LL, 430156615581LL, 733165223741LL, 1675102239LL, 11672282405LL, 28505902370LL, 56850971126LL, 104579441095LL, 184946395485LL, 320271230838LL, 548136169270LL, 931823538043LL, 5566355992LL, 18224518851LL, 39538799199LL, 75428568998LL, 135861084064LL, 237619576909LL, 408964272243LL, 697480775891LL, 975217994LL, 10493791624LL, 26521516204LL, 53509588618LL, 98953098220LL, 175472552088LL, 304318824725LL, 521274920293LL, 886593578634LL, 4681412172LL, 16734417905LL, 37029712447LL, 71203676325LL, 128747054191LL, 225640711843LL, 388793818193LL, 663517023083LL, 309869707LL, 9373453739LL, 24635050076LL, 50333087646LL, 93604389080LL, 166466199041LL, 289153595935LL, 495739149989LL, 843595508858LL, 3837994285LL, 15314239695LL, 34638364011LL, 67177035725LL, 121966848704LL, 214223952145LL, 369569858096LL, 631147011639LL, 1071600249682LL, 8305995893LL, 22837625344LL, 47306517774LL, 88508139307LL, 157884945920LL, 274704166083LL, 471408668248LL, 802626948069LL, 3035492055LL, 13962956971LL, 32363024571LL, 63345735526LL, 115515564461LL, 203361042334LL, 351278490891LL, 600347334415LL, 1019738619303LL, 7289527642LL, 21126058530LL, 44424518524LL, 83655322856LL, 149713595145LL, 260944944925LL, 448240385774LL, 763615341431LL, 2270709524LL, 12675187948LL, 30194631670LL, 59694515740LL, 109367506315LL, 193008715815LL, 333846862571LL, 570995317327LL, 970314612870LL, 6320618749LL, 19494574132LL, 41677365020LL, 79029564632LL, 141924573012LL, 247829502762LL, 426156121163LL, 726429044072LL, 1543147769LL, 11450092757LL, 28131771252LL, 56220995526LL, 103518664987LL, 183160221486LL, 317263605330LL, 543071818099LL, 923295997003LL, 5400022351LL, 17944440140LL, 39067192376LL, 74634460068LL, 134523934228LL, 235368034790LL, 405173042777LL, 691096964198LL, 850140100LL, 10283180861LL, 26166882055LL, 52912442571LL, 97947601772LL, 173779460193LL, 301467934264LL, 516474486013LL, 878510430742LL, 4523053831LL, 16467768161LL, 36580717812LL, 70447642549LL, 127474016798LL, 223497124528LL, 385184367042LL, 657439296860LL, 191205276LL, 9173642154LL, 24298599966LL, 49766560502LL, 92650449706LL, 164859920697LL, 286448885392LL, 491184858787LL, 835926825728LL, 3688703439LL, 15062858246LL, 34215078763LL, 66464292461LL, 120766705411LL, 212203106659LL, 366167084210LL, 625417295778LL, 1061952342872LL, 8117503655LL, 22520235111LL, 46772084390LL, 87608240641LL, 156369663432LL, 272152677710LL, 467112378078LL, 795392696905LL, 2894334002LL, 13725269837LL, 31962798122LL, 62671819558LL, 114380799949LL, 201450284245LL, 348061086052LL, 594929749512LL, 1010616290571LL, 7110982279LL, 20825417357LL, 43918287925LL, 82802913244LL, 148278276422LL, 258528102567LL, 444170817333LL, 756762852279LL, 2136600313LL, 12449369927LL, 29814390879LL, 59054252334LL, 108289407161LL, 191193372862LL, 330790121231LL, 565848263464LL, 961647814278LL, 6151573273LL, 19209929176LL, 41198069474LL, 78222509313LL, 140565623720LL, 245541254027LL, 422303083755LL, 719941157599LL};
long long b1[333] = {270109LL, 270445LL, 268264LL, 272998LL, 273323LL, 271977LL, 273063LL, 273484LL, 271067LL, 276059LL, 271385LL, 273951LL, 274171LL, 274088LL, 274344LL, 272600LL, 272865LL, 273162LL, 269507LL, 269389LL, 275290LL, 274162LL, 274531LL, 273378LL, 271704LL, 270943LL, 267162LL, 273295LL, 274465LL, 272833LL, 273240LL, 274569LL, 271311LL, 273268LL, 275153LL, 275016LL, 274472LL, 274479LL, 273352LL, 273935LL, 273436LL, 274433LL, 276549LL, 270927LL, 268458LL, 273463LL, 271526LL, 272763LL, 276138LL, 274266LL, 274025LL, 268755LL, 277236LL, 272254LL, 273803LL, 274315LL, 275505LL, 274433LL, 273917LL, 275136LL, 270685LL, 273459LL, 271557LL, 272066LL, 272322LL, 272579LL, 271826LL, 273217LL, 274917LL, 273788LL, 275039LL, 271035LL, 274327LL, 273778LL, 271606LL, 274642LL, 274194LL, 273200LL, 274263LL, 271644LL, 268927LL, 275129LL, 271498LL, 274029LL, 273813LL, 273042LL, 270981LL, 273943LL, 273616LL, 271036LL, 272741LL, 270688LL, 274196LL, 273000LL, 271720LL, 277957LL, 274274LL, 274121LL, 273707LL, 273052LL, 272563LL, 273351LL, 269819LL, 264344LL, 269095LL, 272670LL, 276043LL, 271529LL, 273175LL, 274599LL, 273599LL, 273805LL, 273672LL, 276544LL, 273802LL, 271845LL, 274473LL, 273344LL, 274639LL, 271244LL, 276815LL, 274528LL, 268096LL, 270803LL, 272949LL, 273335LL, 270963LL, 272656LL, 273169LL, 272265LL, 272475LL, 276060LL, 273515LL, 274404LL, 271278LL, 271692LL, 272367LL, 274562LL, 266885LL, 274685LL, 271521LL, 273172LL, 272726LL, 272780LL, 273350LL, 270493LL, 274748LL, 276584LL, 272482LL, 277482LL, 274382LL, 270866LL, 273100LL, 273640LL, 277036LL, 274798LL, 277210LL, 267139LL, 272918LL, 271772LL, 272856LL, 273554LL, 274464LL, 271319LL, 269358LL, 274000LL, 273148LL, 274106LL, 274817LL, 274745LL, 272578LL, 271497LL, 269883LL, 275894LL, 272304LL, 275033LL, 273418LL, 273168LL, 272727LL, 278692LL, 268311LL, 273322LL, 276753LL, 272996LL, 271107LL, 276327LL, 276970LL, 273094LL, 273806LL, 275471LL, 270510LL, 273232LL, 274469LL, 274227LL, 271733LL, 273097LL, 271977LL, 268301LL, 265774LL, 273053LL, 272961LL, 273323LL, 274601LL, 275245LL, 272584LL, 278166LL, 273881LL, 275496LL, 276802LL, 274567LL, 273115LL, 273772LL, 274569LL, 273408LL, 276066LL, 269663LL, 271922LL, 274337LL, 275872LL, 274312LL, 272063LL, 273802LL, 272044LL, 277851LL, 275005LL, 273896LL, 274077LL, 270937LL, 271827LL, 271718LL, 273093LL, 272911LL, 276419LL, 275227LL, 270924LL, 271964LL, 275350LL, 272737LL, 274059LL, 281060LL, 273694LL, 272638LL, 270273LL, 270172LL, 273255LL, 273177LL, 274216LL, 274229LL, 273457LL, 267160LL, 275332LL, 273104LL, 271477LL, 272622LL, 272016LL, 274012LL, 271062LL, 279518LL, 273478LL, 273447LL, 273141LL, 271313LL, 274015LL, 274000LL, 272541LL, 271358LL, 273661LL, 270376LL, 269305LL, 273036LL, 274669LL, 270484LL, 273333LL, 270707LL, 272237LL, 272434LL, 275526LL, 271751LL, 272658LL, 271852LL, 273706LL, 274036LL, 278926LL, 271438LL, 272189LL, 273162LL, 274348LL, 272400LL, 273064LL, 273294LL, 272998LL, 265573LL, 275185LL, 269736LL, 274911LL, 273704LL, 274303LL, 271223LL, 273027LL, 279806LL, 269188LL, 273943LL, 273942LL, 273504LL, 271830LL, 274794LL, 274159LL, 272102LL, 276317LL, 272257LL, 271802LL, 274778LL, 271186LL, 271781LL, 273338LL, 273487LL, 273060LL, 272431LL, 270078LL, 270738LL, 271057LL, 273023LL, 271789LL, 272753LL, 272111LL, 270229LL, 274027LL, 273329LL, 272610LL, 273459LL, 272491LL, 274246LL, 273272LL};
long long s1[333] = {8852533278LL, 23757904982LL, 48856118466LL, 91117413889LL, 162278538675LL, 282102259298LL, 483865849913LL, 823602809260LL, 3445957101LL, 14654113012LL, 33526818558LL, 65305374580LL, 118815276790LL, 208917219374LL, 360634186316LL, 616100798910LL, 1046264882565LL, 7809943159LL, 22002353394LL, 45900056014LL, 86139887264LL, 153897195873LL, 267989445841LL, 460102174919LL, 783588658832LL, 2662884633LL, 13335546904LL, 31306568294LL, 61566835623LL, 112520187313LL, 198317316178LL, 342785679079LL, 586046825043LL, 995658895436LL, 6817885353LL, 20331889933LL, 43087268176LL, 81403611615LL, 145922080769LL, 254560653738LL, 437490280594LL, 745513918503LL, 1916713887LL, 12079117113LL, 29190945631LL, 58004472350LL, 106521748941LL, 188216924844LL, 325778268623LL, 557409121770LL, 947437676753LL, 5872487458LL, 18739994132LL, 40406775470LL, 76890099252LL, 138322062702LL, 241763463296LL, 415941901074LL, 709229964324LL, 1205175845LL, 10881003134LL, 27173517181LL, 54607451977LL, 100801720992LL, 178585331327LL, 309560237000LL, 530100603167LL, 901454588131LL, 4972160342LL, 17223990388LL, 37854072609LL, 72591764348LL, 131084367894LL, 229576366682LL, 395420818594LL, 674675810509LL, 528530419LL, 9741642696LL, 25255020209LL, 51377016234LL, 95362194476LL, 169426056398LL, 294137512729LL, 504131252606LL, 857726439846LL, 4114790796LL, 15780319918LL, 35423167105LL, 68498516212LL, 124192006144LL, 217970754158LL, 375878861249LL, 641770343149LL, 1089488206171LL, 8655861163LL, 23426741255LL, 48298492751LL, 90178463236LL, 160697498653LL, 279440045945LL, 479383116827LL, 816054618345LL, 3299010547LL, 14406679142LL, 33110180309LL, 64603823927LL, 117633979866LL, 206928108236LL, 357284847682LL, 610461059075LL, 1036768481331LL, 7623086491LL, 21687717378LL, 45370260351LL, 85247797846LL, 152395063105LL, 265460099409LL, 455843168584LL, 776417186988LL, 2521783407LL, 13097955489LL, 30906503230LL, 60893191334LL, 111385880203LL, 196407328042LL, 339569570755LL, 580631423798LL, 986540243611LL, 6638964596LL, 20030616582LL, 42579973080LL, 80549409317LL, 144483743389LL, 252138728185LL, 433412153289LL, 738647018285LL, 1782403767LL, 11852960802LL, 28810135294LL, 57363249837LL, 105442034804LL, 186398862532LL, 322716948231LL, 552254357781LL, 938757895651LL, 5702492293LL, 18453750043LL, 39924787216LL, 76078509626LL, 136955478121LL, 239462358003LL, 412067214799LL, 702705625076LL, 1078245152LL, 10667272549LL, 26813629760LL, 54001460305LL, 99781329939LL, 176867159261LL, 306667115638LL, 525229058851LL, 893251702379LL, 4811417271LL, 16953325216LL, 37398316609LL, 71824345590LL, 129792160158LL, 227400499501LL, 391757013574LL, 668506560506LL, 407472910LL, 9537801541LL, 24911784919LL, 50799063936LL, 94389016988LL, 167787383973LL, 291378255676LL, 499485113660LL, 849903100442LL, 3962215626LL, 15523408161LL, 34990569599LL, 67770092461LL, 122965459512LL, 215905449939LL, 372401226061LL, 635914573220LL, 1079628045290LL, 8463249120LL, 23102414126LL, 47752378861LL, 89258896620LL, 159149098713LL, 276832793068LL, 474992928732LL, 808662257728LL, 3153747609LL, 14162079962LL, 32698315149LL, 63910310096LL, 116466215854LL, 204961784137LL, 353973878764LL, 604885928085LL, 1027380870279LL, 7439757458LL, 21379021075LL, 44850466087LL, 84372549087LL, 150921286733LL, 262978500506LL, 451664560707LL, 769381093408LL, 2384479369LL, 12866757783LL, 30517204006LL, 60237674862LL, 110282097477LL, 194548738044LL, 336440008515LL, 575361751833LL, 977666976140LL, 6465630114LL, 19738749701LL, 42088516872LL, 79721877399LL, 143090314774LL, 249792421742LL, 429461356209LL, 731994520153LL, 1651466156LL, 11632483285LL, 28438887209LL, 56738128651LL, 104389432799LL, 184626452361LL, 319732498807LL, 547229031940LL, 930296066835LL, 5536910560LL, 18174937413LL, 39455312146LL, 75287990224LL, 135624372471LL, 237220993197LL, 408293122306LL, 696350669280LL, 953808708LL, 10457741857LL, 26460814318LL, 53407376623LL, 98780989914LL, 175182749916LL, 303830845070LL, 520453242117LL, 885210006694LL, 4654232630LL, 16688651901LL, 36952649891LL, 71073915644LL, 128528558655LL, 225272800811LL, 388174316509LL, 662473883267LL, 289206832LL, 9338660822LL, 24576464478LL, 50234439059LL, 93438280947LL, 166186500050LL, 288682628421LL, 494946117563LL, 842260171658LL, 3811835984LL, 15270193356LL, 34564197274LL, 67052151036LL, 121756563326LL, 213869865896LL, 368973634826LL, 630143069007LL, 1069909773689LL, 8273409685LL, 22782755450LL, 47214125862LL, 88352566434LL, 157622986731LL, 274263069721LL, 470665933990LL, 801376304810LL, 3010754047LL, 13921302128LL, 32292884545LL, 63227631034LL, 115316695627LL, 203026179741LL, 350714637021LL, 599397896633LL, 1018139921109LL, 7258921846LL, 21074523434LL, 44337741800LL, 83509205083LL, 149467556420LL, 260530655991LL, 447542790159LL, 762440704743LL, 2248562362LL, 12637895718LL, 30131837544LL, 59588780626LL, 109189465414LL, 192708923851LL, 333342061747LL, 570145314776LL, 968883347547LL, 6293422768LL, 19448780527LL, 41600256045LL, 78899725828LL, 141705945710LL, 247461370414LL, 425536246138LL, 725385275365LL, 1522326802LL, 11415033620LL, 28072737455LL, 56121592204LL, 103351286100LL, 182878382780LL, 316789034914LL, 542272719099LL, 921950444746LL};

int main() {

	long long N, res=0;
	cin >> N;
	if(N >= start){
		res += beg;
		N -= start;
		res += loop * (N / period);
		N %= period;
		long long i = 0;
		while(N >= block) {
			res += b1[i++];
			N -= block;
		}
		long long s = i==0 ? sl : f(s1[i-1]);
		for(i = 0; i < N; ++i) {
			if(s%2==0) ++res;
			s = f(s);
		}
	} else {
		long long i = 0;
		while(N >= block) {
			res += b0[i++];
			N -= block;
		}
		long long s = i==0 ? S0 : f(s0[i-1]);
		for(i = 0; i < N; ++i) {
			if(s%2==0) ++res;
			s = f(s);
		}
	}
	cout << res << endl;

	return 0;
}