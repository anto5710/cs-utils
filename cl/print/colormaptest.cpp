#include "color_map.h"
#include "color_stream.h"
#include "color_tag.h"
#include "string_util.h"
#include <iostream>


#include <chrono>
#include <cmath>
#include <iomanip>
#include <stdio.h>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>

using namespace std;

int main()
{
    ANSISequence s = R_BOLD;
    // std::cout << s << "THIS";

    // std::cout << U_UNDO;
    ColorStream cs(std::cout);
    cs << s << "THIS" << s << "inter" << R_RESET
       << mini_color((C_GREEN + ST_BOLD), "TEST") << "asddsad" << std::endl;

    std::string ff = apply_tags("<g>sdsd <u>sdsd</g></u>"
                                "<i><b><b></*>sd<r>sdsd</*><2>");


    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf("lines %d\n", w.ws_row);
    printf("columns %d\n", w.ws_col);

    // ColorStream cs(cout);

    cs << R_RESET;

    int ww = 3;
    int r = 255, g = 255, b = 255;

    system("clear");
    cs << "\e[s" << flush; ///< saves current cursor pos

    int i = 0;

    string loremipsum =
        "Wookieepedia. Wookieepedia. EXPLORE. TRENDING. STAR WARS. NAVIGATION. "
        "COMMUNITY.  FANDOM. GAMES. MOVIES. TV. VIDEO. WIKIS. START A WIKI. "
        "ADVERTISEMENT. Wookieepedia. In a galaxy far, far away..... 173,530. "
        "PAGES. EXPLORE. TRENDING. STAR WARS. NAVIGATION. COMMUNITY. . . in: "
        "Legends articles, Changes of government, Galactic Republic laws and "
        "acts, and 2 more. Declaration of a New Order. This article details a "
        "subject that falls under the Legends brand.. EDIT. Click here for "
        "Wookieepedia's article on the Canon version of this subject.	 	"
        "This article covers the Legends version of this subject.	 . Related "
        "events. Clone Wars. Empire Day. Great Jedi Purge. Proposed by. "
        "Supreme Chancellor Palpatine. . Declaration of a New Order. Date. 19 "
        "BBY (16:5:23[1]). Government. Galactic Republic. Legislative body. "
        "Galactic Senate. Outcome. The Galactic Republic becomes the Galactic "
        "Empire via the New Order.. Supreme Chancellor Palpatine ascends to "
        "the position of Galactic Emperor in a life-long term.. The Galactic "
        "Senate becomes the Imperial Senate.. The Clone Wars are officially "
        "brought to an end.. The Jedi are declared enemies of state.. "
        "[Source]. \"So this is how liberty dies... with thunderous "
        "applause.\". ―Padmé Amidala — (audio) Listen (file info)[src]. The "
        "Declaration of a New Order was a proclamation made by Supreme "
        "Chancellor Palpatine in 19 BBY, first as an address to an "
        "Extraordinary Session of the Galactic Senate and then to the general "
        "populace. In it, he proclaimed himself Emperor of the galaxy, "
        "legalized the extermination of the Jedi Order, and outlined his "
        "vision of transforming the thenceforth-abolished Galactic Republic "
        "into his new Galactic Empire.. . The anniversary of the Declaration "
        "of a New Order was celebrated as Empire Day.. . . Contents. 1 Full "
        "text. 1.1 Reactions to the speech. 2 Behind the scenes. 3 "
        "Appearances. 4 Sources. 5 Notes and references. Full text. Source:  "
        "Republic HoloNet News Special Inaugural Edition 16:5:241	"
        "Attribution:  Daniel Wallace, Pablo Hidalgo. Citizens of the "
        "civilized galaxy, on this day we mark a transition. For a thousand "
        "years, the Republic stood as the crowning achievement of civilized "
        "beings. But there were those who would set us against one another, "
        "and we took up arms to defend our way of life against the "
        "Separatists. In so doing, we never suspected that the greatest threat "
        "came from within.. The Jedi, and some within our own Senate, had "
        "conspired to create the shadow of Separatism using one of their own "
        "as the enemy's leader. They had hoped to grind the Republic into "
        "ruin. But the hatred in their hearts could not be hidden forever. At "
        "last, there came a day when our enemies showed their true natures.. . "
        "The Jedi hoped to unleash their destructive power against the "
        "Republic by assassinating the head of government and usurping control "
        "of the clone army. But the aims of would-be tyrants were valiantly "
        "opposed by those without elitist, dangerous powers. Our loyal clone "
        "troopers contained the insurrection within the Jedi Temple and "
        "quelled uprisings on a thousand worlds.. . The remaining Jedi will be "
        "hunted down and defeated! Any collaborators will suffer the same "
        "fate. These have been trying times, but we have passed the test. The "
        "attempt on my life has left me scarred and deformed, but I assure you "
        "my resolve has never been stronger. The war is over. The Separatists "
        "have been defeated, and the Jedi rebellion has been foiled. We stand "
        "on the threshold of a new beginning. In order to ensure our security "
        "and continuing stability, the Republic will be reorganized into the "
        "first Galactic Empire, for a safe and secure society, which I assure "
        "you will last for ten thousand years. An Empire that will continue to "
        "be ruled by this august body and a sovereign ruler chosen for life. "
        "An Empire ruled by the majority, ruled by a new constitution!. . By "
        "bringing the entire galaxy under one law, one language, and the "
        "enlightened guidance of one individual, the corruption that plagued "
        "the Republic in its later years will never take root. Regional "
        "governors will eliminate the bureaucracy that allowed the Separatist "
        "movement to grow unchecked. A strong and growing military will ensure "
        "the rule of law.. . Under the Empire's New Order, our most cherished "
        "beliefs will be safeguarded. We will defend our ideals by force of "
        "arms. We will give no ground to our enemies and will stand together "
        "against attacks from within or without. Let the enemies of the Empire "
        "take heed: those who challenge Imperial resolve will be crushed.. . "
        "We have taken on a task that will be difficult, but the people of the "
        "Empire are ready for the challenge. Because of our efforts, the "
        "galaxy has traded war for peace and anarchy for stability. Billions "
        "of beings now look forward to a secure future. The Empire will grow "
        "as more planets feel the call, from the Rim to the wilds of unknown "
        "space.. . Imperial citizens must do their part. Join our grand star "
        "fleet. Become the eyes of the Empire by reporting suspected "
        "insurrectionists. Travel to the corners of the galaxy to spread the "
        "principles of the New Order to barbarians. Build monuments and "
        "technical wonders that will speak of our glory for generations to "
        "come.. . The clone troopers, now proudly wearing the name of Imperial "
        "stormtroopers, have tackled the dangerous work of fighting our "
        "enemies on the front lines. Many have died in their devotion to the "
        "Empire. Imperial citizens would do well to remember their example.. . "
        "The New Order of peace has triumphed over the shadowy secrecy of "
        "shameful magicians. The direction of our course is clear. I will lead "
        "the Empire to glories beyond imagining.. . We have been tested, but "
        "we have emerged stronger. We move forward as one people: the Imperial "
        "citizens of the first Galactic Empire. We will prevail. Ten thousand "
        "years of peace begins today.. This work is copyrighted. The "
        "individual who uploaded this work asserts that this qualifies as fair "
        "use of the material under United States copyright law.. Reactions to "
        "the speech. Although the speech received unanimous signatures in "
        "support of transitioning the Galactic Republic into the Galactic "
        "Empire, the speech itself had mixed reception. Several of the "
        "senators were in support of it because it meant a chance at a time of "
        "peace after three years of the Clone Wars and the carnage that "
        "occurred. Several of these senators also felt it would solidify their "
        "power base. Others, notably the senators who signed the Delegation of "
        "2000, personally disagreed with the speech, but felt the need to "
        "support it for their own safety, because of armed men inside the room "
        "who would shoot if they dared speak out. During the speech, the Naboo "
        "senator representing the Chommell sector, Padmé Amidala, mused that "
        "\"to thunderous applause\" was \"how liberty dies\".[2] Mon Mothma, "
        "the Chandrilan senator and later one of the top leaders of the "
        "Alliance to Restore the Republic, also reflected later on that she "
        "never believed a word of Palpatine's speech, especially his remarks "
        "about the Jedi trying to commit treason against him, and also "
        "expressed disgust at the fellow senators applauding Palpatine's "
        "\"silver lies.\"[3] The full text of the speech was eventually "
        "printed out during the Republic HoloNet News Special Inaugural "
        "Edition 16:5:241, which was ultimately the last HoloNews "
        "broadcast.[1] The speech was also frequently excerpted within the "
        "official field guide for the Imperial Military, the Imperial "
        "Handbook: A Commander's Guide, released nineteen years after the "
        "Declaration. Palpatine, in his introduction to the Imperial Handbook, "
        "likewise admitted that the Declaration of a New Order and likewise "
        "the resulting shift from the Republic to the Empire was an "
        "unprecedented shift of power in galactic history, but also mentioned "
        "the people had been desperate and that the Republic could not "
        "continue on its current course, and also implied that he acted on the "
        "people's behalf when he made the decision for the Senate.[3]. . "
        "Behind the scenes. . Palpatine announcing the birth of the New "
        "Order.. . Some elements of this text contradict the text found in the "
        "novelization of the 2005 film Star Wars: Episode III Revenge of the "
        "Sith, such as the \"Safety, security, justice and peace!\" chant. The "
        "author of the Insider article admitted that he had forgotten to check "
        "the novelization before writing his article. In addition, some parts "
        "of the text were switched around from the film (for example, in the "
        "article, the phrase \"And the Jedi rebellion has been foiled.\" took "
        "place after \"The remaining Jedi will be hunted down and defeated.\" "
        "In the film, it was in reverse order).. . It should be noted, "
        "however, that the novelization text differs from what is seen in the "
        "film. The segment seen in the novel goes as follows:. . Source:  Star "
        "Wars Episode III: Revenge of the Sith (novel)	Attribution:  Matthew "
        "Stover. These Jedi murderers left me scarred, left me deformed, but "
        "they could not scar my integrity! They could not deform my resolve! "
        "The remaining traitors will be hunted down, rooted out wherever they "
        "may hide, and brought to justice, dead or alive! All collaborators "
        "will suffer the same fate. Those who protect the enemy are the enemy! "
        "Now is the time! Now we will strike back! Now we will destroy the "
        "destroyers! Death to the enemies of the democracy!. This has been the "
        "most trying of times, but we have passed the test. The war is over!. "
        ". The Separatists have been utterly defeated, and the Republic will "
        "stand. United! United and free!. . The Jedi Rebellion was our final "
        "test—it was the last gasp of the forces of darkness! Now we have left "
        "that darkness behind us forever, and a new day has begun! It is "
        "morning in the Republic!. . Never again will we be divided! Never "
        "again will sector turn against sector, planet turn against planet, "
        "sibling turn against sibling. We are one nation, indivisible!. . To "
        "ensure that we will always stand together, that we will always speak "
        "with a single voice and act with a single hand, the Republic must "
        "change. We must evolve. We must grow. We have become an empire in "
        "fact; let us become an Empire in name as well! We are the first "
        "Galactic Empire!. . We are an Empire that will continue to be ruled "
        "by this august body! We are an Empire that will never return to the "
        "political manoeuvring and corruption that have wounded us so deeply; "
        "we are an Empire that will be directed by a single sovereign, chosen "
        "for life!. . We are an Empire ruled by the majority! An Empire ruled "
        "by a new Constitution! An Empire of laws, not of politicians! An "
        "Empire devoted to the preservation of a just society. Of a safe and "
        "secure society! We are an Empire that will stand ten thousand years!. "
        ". We will celebrate the anniversary of this day as Empire Day. For "
        "the sake of our children. For our children's children! For the next "
        "ten thousand years! Safety! Security! Justice and peace!. . Say it "
        "with me! Safety, Security, Justice, and Peace! Safety, Security, "
        "Justice, and Peace!. This work is copyrighted. The individual who "
        "uploaded this work asserts that this qualifies as fair use of the "
        "material under United States copyright law.. The film version of this "
        "text goes as follows:. . Source:  Star Wars: Episode III Revenge of "
        "the Sith	Attribution:  George Lucas. And the Jedi rebellion has "
        "been foiled.. . The remaining Jedi will be hunted down and defeated.. "
        ". […]. . The attempt on my life has left me scarred and deformed. "
        "But, I assure you, my resolve has never been stronger.. . […]. . In "
        "order to ensure our security and continuing stability, the Republic "
        "will be reorganized into the first Galactic Empire for a safe and "
        "secure society.. This work is copyrighted. The individual who "
        "uploaded this work asserts that this qualifies as fair use of the "
        "material under United States copyright law.. In the DVD commentary of "
        "Revenge of the Sith, George Lucas stated that the interwoven scenes "
        "of the Declaration of a New Order and the Mission to Mustafar was an "
        "homage to the christening and assassination sequence of the 1972 "
        "classic film The Godfather, directed by his friend Francis Ford "
        "Coppola.[4]. . According to Lucas, the declaration, as well as Padmé "
        "Amidala's comment about liberty dying being \"from thunderous "
        "applause\", was intended to reference the rise of Nazi leader Adolf "
        "Hitler in Germany. Similarly, Palpatine's line in both the "
        "novelization and the full speech included in Star Wars Insider about "
        "his new Empire lasting 10,000 years was similar to Hitler's vow of a "
        "\"Thousand-year Reich\", while in the end, both the Galactic Empire "
        "and the Third Reich were relatively short-lived (though the Galactic "
        "Empire actually lasted longer than the Third Reich).. . In the "
        "novelization, during his speech, Palpatine stated that it's \"Morning "
        "in the Republic\", which was similar to president Ronald Reagan's "
        "1984 campaign ad \"Morning in America.\". . Appearances. TCW mini "
        "logo.jpg Star Wars: The Clone Wars – \"Carnage of Krell\" (Indirect "
        "mention only). Star Wars: Episode III Revenge of the Sith (First "
        "appearance). Star Wars Episode III: Revenge of the Sith comic. Star "
        "Wars: Episode III Revenge of the Sith novelization. Star Wars: "
        "Episode III Revenge of the Sith unabridged audiobook. Star Wars "
        "Episode III: Revenge of the Sith junior novel. SWInsider.png "
        "\"Republic HoloNet News Special Inaugural Edition 16:5:241\"—Star "
        "Wars Insider 84. SWAJsmall.jpg \"Mist Encounter\"—Star Wars Adventure "
        "Journal 7 (Mentioned only). Purge – The Tyrant's Fist 2 (Mentioned "
        "only). Dark Lord: The Rise of Darth Vader (Mentioned only). The Last "
        "of the Jedi: Reckoning (Mentioned only). Coruscant Nights II: Street "
        "of Shadows (Indirect mention only). The Force Unleashed II novel "
        "(Mentioned only). SWG logo sm.png Star Wars Galaxies: An Empire "
        "Divided (Mentioned only). Millennium Falcon (Mentioned only). "
        "Millennium Falcon audiobook (Mentioned only). Crucible (Indirect "
        "mention only). Crucible audiobook (Indirect mention only). Sources. "
        "Imperial Sourcebook. Heir to the Empire Sourcebook. Dark Force Rising "
        "Sourcebook. Galaxy Guide 6: Tramp Freighters, Second Edition. "
        "Imperial Sourcebook, Second Edition. Shadows of the Empire "
        "Sourcebook. Kenner-logo.svg Star Wars: Shadows of the Empire (Pack: "
        "Darth Vader vs. Prince Xizor) (backup link) (Indirect mention only). "
        "Galoob.svg Star Wars: Shadows of the Empire Micro Machines (Pack: "
        "Collection II (backup link) (Indirect mention only). The Thrawn "
        "Trilogy Sourcebook. Player's Guide to Tapani. Star Wars: Rebellion: "
        "Prima's Official Strategy Guide (Indirect mention only). The Force "
        "Unleashed Campaign Guide. SWGTCGsmall.jpg Star Wars Galaxies Trading "
        "Card Game – Threat of the Conqueror (Card: Empire's New Order). The "
        "Essential Guide to Warfare. Star Wars: Edge of the Empire Core "
        "Rulebook. Suns of Fortune. SWInsider.png \"Rogues Gallery: The Cloud "
        "City Wing Guards\"—Star Wars Insider 148 (Indirect mention only). "
        "StarWars.com Encyclopedia Mas Amedda in the Encyclopedia (content now "
        "obsolete; backup link) (Indirect mention only). Star Wars: Imperial "
        "Handbook: A Commander's Guide. Fly Casual. Strongholds of Resistance. "
        "Notes and references.  SWInsider.png \"Republic HoloNet News Special "
        "Inaugural Edition 16:5:241\"—Star Wars Insider 84.  Star Wars: "
        "Episode III Revenge of the Sith.  Star Wars: Imperial Handbook: A "
        "Commander's Guide.  Star Wars: Episode III Revenge of the Sith DVD "
        "commentary. Elections. Galactic Republic Supreme Chancellor "
        "elections. 3900 BBY · 3653 BBY · 3641 BBY · 40 BBY · 36 BBY · 32 BBY "
        "· 28 BBY. New Republic Chief of State elections. 11 ABY · 21 ABY · 23 "
        "ABY · 28 ABY. Galactic Alliance Chief of State elections. 44 ABY(a) · "
        "44 ABY(b). Imperial Remnant Head of State elections. 44 ABY. Other. "
        "Declaration of a New Order · Jamaane coup. Solo-Niathal coup · Jedi "
        "coup of the Galactic Alliance · Sith coup of the Fel Empire. [hide] "
        "[edit]. In other languages. "
        "češtinaΕλληνικάespañolpolskiportuguêsрусскийsuomi. Categories. "
        "Community content is available under CC-BY-SA unless otherwise "
        "noted.. Popular Pages. . Cad Bane. . Darth Malgus. . Boba Fett. . "
        "Darksaber. . Din Djarin. EXPLORE PROPERTIES. Fandom. D&D Beyond. "
        "Cortex RPG. Muthead. Futhead. Fanatical. FOLLOW US. OVERVIEW. What is "
        "Fandom?. About. Careers. Press. Contact. Terms of Use. Privacy "
        "Policy. Global Sitemap. Local Sitemap. COMMUNITY. Community Central. "
        "Support. Help. ADVERTISE. Media Kit. Fandomatic. Contact. FANDOM "
        "APPS. Take your favorite fandoms with you and never miss a beat.. "
        "Wookieepedia is a FANDOM Movies Community.. VIEW MOBILE SITE. Follow "
        "on IG. Newsletter. Join Fan Lab. Bingebot: Find a new show to watch "
        "in 30 seconds";


    int mi = loremipsum.length();

    while (true) {


        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        int dr = 12, dg = 1, db = 1;


        int width  = w.ws_col;
        int height = w.ws_row;

        for (int y = 0; y < height - 3; y++) {
            for (int x = 0; x < width; x++) {

                stringstream ss;

                // r = (r + dr);
                // g = (g + dg);
                // b = (b + db);

                // if (r > 255 || r < 0) {
                //     dr *= -1;
                // }
                // if (g > 255 || g < 0) {
                //     dg *= -1;
                // }
                // if (b > 255 || b < 0) {
                //     db *= -1;
                // }

                // r = min(
                //     255,
                //     max(((int)round(abs(sin(x * 23 + y) * 2132)) % 256), 0));
                // g = min(
                //     255,
                //     max(((int)round(abs(cos(x * 23 + y) * 1132)) % 256), 0));
                // b = min(
                //     255,
                //     max(((int)round(abs(atan(x * 23 + y) * 232)) % 256), 0));

                ss << "48;2;" << ((r + x) % 256) << ";" << ((g + y) % 256)
                   << ";" << ((b + r - g) % 255);
                // ss << "48;2" << r << ";" << g << ";" << b;

                cs << setw(ww)
                   << mini_color(ANSISequence("30;" + ss.str(), "RGB seq"),
                                 string(1, loremipsum[(i++) % mi]))
                   << R_RESET;


                // std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            cs << endl;
        }


        cs << "\e[u\e[F"; ///< return to saved cursor pos (i.e., top left)
        // cs << "\e[J"; ///< flush everything below
    }

    // std::cout << ff << std::endl;

    // // spliterate(";;;;.", ";");

    cout << "\e[38;";

    // cout <<
}