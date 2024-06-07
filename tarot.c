#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define TOTAL_CARDS 78
#define MAX_CARD_VALUE 6
#define INITIAL_DRAW 1
#define MUM_MESSAGES 31
#define RED_TEXT "\033[0;31m"
#define GRAY_TEXT "\033[2;32m"
#define YELLOW_TEXT "\033[0;33m"
#define BLUE_TEXT "\033[0;34m"
#define TEST_TEXT "\033[1m"
#define WHITE_BG_TEXT "\033[30;42m"
#define RESET_TEXT "\033[0m"

void clearScreen() {
printf("\033[2J\033[1;1H");
}

const char *goodbyeMessages[] = {
	"Goodbye, and may the old gods bless you!",
	"Farewll, and take care friend!",
        "Thank your for passing by!",
        "Farewll, cowboy!",
        "Your lucky runs thin today.",
        "Goodluck today..",
        "May the luck of the old gods be with you.",
        "Be safe friend.",
        "Thanks for your time buddy. Smoke break jozeppio!",
        "I can't take your money this ones is on me.",
        "Before you go, you lost some coins from your pocket.",
        "Until we meet again, stay safe crossing the railroad tracks.",
        "Until we meet again, stay safe!",
        "Wishing you all the best. Goodbye!",
        "May your journey ahead be illuminated tonight.",
        "See you later, alligator!",
        "Remember the hand's may be quicker than the eye but it still leaves behind fingerprints.",
        "Catch you on the flip flop!",
        "May your life be a beautiful symphony of laughter, love, and unforgettable moments.",
	"Goodday & thanks for your coins.",
        "May each sunrise bring you hope.",
        "May your days be sprinkled with kindness much like the rain gloom today.",
        "When you find out your pulmonologist smokes. Well that's ironic..",
        "4 quarters; here have a dollar back thanks Mr.",
        "Sometimes life just kicks you down.",
        "The outlook is not so good!",
        "The cards have the key to self and I..Until we meet again!",
        "All is not hopeless with help of the cards.",
        "Magicians are the most honest people in the world; they till you they're gonna fool you, and then they to it.",
        "You don't get into magic. Magic gets into you. goodbye then.",
	"Weak people talk and do not act,  as strong people act and keep quiet."
};

void initializeRandom() {
srand(time(NULL) ^ (getpid() << 16) ^ clock());
}

void greetBasedOnTime() {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    int hour = timeinfo->tm_hour;

    if (hour >= 4 && hour < 12) {
        printf("Good morning!\n");
    } else if (hour >= 12 && hour < 18) {
        printf("Good afternoon!\n");
    } else if (hour >= 18 && hour < 22) {
        printf("Good evening!\n");
    } else {
        printf(" ");
    }
}

void determineCardState() {
initializeRandom();
int randomNumber = rand() % TOTAL_CARDS;
if (randomNumber < TOTAL_CARDS / 2) {
printf("the card is ");
printf(BLUE_TEXT);
printf("upright");
printf(RESET_TEXT);
printf(".\n\n");
} else {
printf("the card is ");
printf(BLUE_TEXT);
printf("reversed");
printf(RESET_TEXT);
printf(".\n\n");
}
}

void busted() {
    time_t now;
    struct tm *current_time;

    time(&now);
    current_time = localtime(&now);

    if (current_time->tm_hour == 12 && current_time->tm_min == 55) {
        clearScreen();
        printf(TEST_TEXT);
        printf("Imagine the unsettling moment when the blue and red lights of law enforcement suddenly flood the dimly lit street, signaling the arrival of the police. \n");
        printf("As you stand there, puzzled and anxious, the officers approach with purposeful strides, their authoritative demeanor unmistakable.\n ");
        printf("With a stern yet solemn expression, they inform you of the reason for their visit: suspicion of loitering.\n");
        printf("Time seems to stand still as you grapple with the surreal reality of the situation. The abruptness of the arrest, coupled with the unexpectedness of the accusation, leaves you momentarily stunned, your mind racing to comprehend the gravity of the circumstances.\n");
        printf("In that fleeting moment, a wave of confusion and disbelief washes over you, as you come to terms with the sudden and unexpected intrusion of the law into your mundane existence.\n");
        printf("In the midst of the chaotic encounter with law enforcement, you find yourself vehemently protesting your innocence, insisting that you were not loitering. However, your pleas fall upon deaf ears as the police, seemingly unmoved by your protestations, proceed with alarming aggression.\n");
        printf("Ignoring your cries, they unleash a torrent of brutal force, raining down blows upon you with savage intensity.\n");
        printf("Amidst the chaos, your eyes catch the chilling sight of the fortune teller, a sinister grin etched upon their face, watching the scene unfold with a perverse satisfaction. As the brutality intensifies, a sense of helplessness envelops you, your world descending into darkness as you succumb to the relentless assault, your consciousness slipping away into oblivion.\n");
        printf(RESET_TEXT);
    }
    exit(0);
}

void random_fortune() {
time_t now2;
struct tm *currentTime;
time(&now2);
currentTime = localtime(&now2);
if (currentTime->tm_hour == 17 && currentTime->tm_min == 0) {
     const char *sayings[] = {
            "Time flies like an arrow; fruit flies like a banana.",
            "Lost time is never found again.",
            "Time is what we want most, but what we use worst.",
            "Time you enjoy wasting is not wasted time.",
            "Better three hours too soon than a minute too late.",
            "The trouble is, you think you have time.",
            "Time is a created thing. To say 'I don't have time,' is like saying, 'I don't want to.'",
            "An inch of time is an inch of gold but you can't buy that inch of time with an inch of gold.",
            "Time is free, but it's priceless.",
            "Time is the wisest counselor of all.",
            "You may delay, but time will not.",
            "The two most powerful warriors are patience and time.",
            "The only reason for time is so that everything doesn't happen at once.",
            "They always say time changes things, but you actually have to change them yourself.",
            "Time is what prevents everything from happening at once.",
            "We have nothing but time today.. "
        };

  int num_sayings = sizeof(sayings) / sizeof(sayings[0]);
  srand(time(NULL));
  int random_index = rand() % num_sayings;
  printf(YELLOW_TEXT);
  printf("fortune teller:");
  printf(RESET_TEXT);
  printf(TEST_TEXT);
  printf("%s\n", sayings[random_index]);
  printf(RESET_TEXT);
}
}

void displayCard(int cardNumber) {
time_t current_time;
struct tm *time_info;

char choice;
    switch (cardNumber) {
        case 0:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("I can give you a simple explanations for the card The Fool (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("Indicating new beginnings, innocence, and adventure.");
            printf("It can also mean foollshness, carelessness, and lack of commitment.\n"); 
            }
            printf("The Fool ");
            usleep(11000);
            break;
        case 1:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("A simple explanations for the card The Magician (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("The Migician is the reoresentation of pure willpower.");
            printf("An ability to act as a go-between in the world. ");
            printf("It can indicate success in love, career, and finances.");
            printf("But also deception and illusion in reversed positions.\n");
            }
            printf("The Magician ");
            usleep(11000);
            break;
        case 2:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("I can give you a simple explanations for the card High Priestess (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("This card is the guardian of the subconscious mind the teacher of sacred knowledge. ");
            printf("She can also signal changes in love, career and finances.");
            printf("The She can indicate that you need to listen to your inner voice,");
            printf("follow your gut instincts and connect with your higher power.");
            }
            printf("High Priestess ");
            usleep(11000);
             break;
        case 3:
            printf(YELLOW_TEXT);
            printf("fortuen teller:");
            printf(RESET_TEXT);
            printf("I can give you more information for the card The Empress (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("Well Money, career, Love & relationships (upright).");
            printf("You are creative and will inspire the people around you with your passions.");
            printf("Financially speaking this is a good time for cash flow. If the card is upright that is...");
            printf("Reversed Insecurity, infertility, lack of confidence, lack of growth, overbearing tendencies, disharmony, negligence.\n");
            }
            printf("The Empress ");
            usleep(11000);
            break;
        case 4:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("I can give you more information on The Emperor card (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("As a person, The Emperor represents an older man who is good in business and usually wealthy. The Emperor is a solid, stable man. He is a powerful, grounded protector but he also can be rigid and stubborn. (Upright)");
            printf("(Reversed) Abuse of power, excessively controlling, rigidity, stubbornness, lack of discipline, lack of control, absentee father, paternity issues..\n");
            }
            printf("The Emperor ");
            usleep(11000);
            break;
        case 5:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("The Hierophant card .. More info its your dime after all (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("Hierophant Tarot card can represent include economic, religious, political, social, family, educational, social welfare and medical. Its appearance can signify that now is a time to conform to convention or tradition. It’s not a time to rock the boat. (Upright)");
            printf("if (Reversed) feel compelled to think for yourself and throw away the rule book. \n");
            }
            printf("The Hierophant ");
            usleep(11000);
            break;
        case 6:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("More info about the Lovers card (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("Well you are lucky today! ");
            printf("The Lovers signifies perfect union, harmony, love and attraction.(Upright)");
            printf("reversed indicates that you may be struggling to take ownership of the decisions you’ve made. This is causing conflict within you. You may be feeling uncertain of the direction your life is going in. Vice! I somke and drink like a fish. ");
            }
            printf("The Lovers ");
            usleep(11000);
            break;
        case 7:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("The Chariot more information about this card (Y/N)? ");
            scanf(" %c", &choice);
         if (choice == 'y') {
        printf("As you wish.\nThe Chariot Tarot represents overcoming obstacles through determination, focus and willpower, Keep in mind this if the code is (Upright),,"); 
        printf("If the card is reversed can indicate that you feel powerless and are lacking direction.");
        printf("When The Chariot card is reversed you are still moving but you have let go of the reins.\n");
        }
            printf("The Chariot ");
            usleep(10000);
            break;
        case 8:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Strength card can I explain more about this (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("(Upright) The Strength Tarot card is the Major Arcana card of inner strength. It represents mastering raw emotions in order to bring calm to yourself or a situation. It is similar to the Chariot in that it represents overcoming challenges. However, the Strength card usually refers to inner challenges. Overcoming the obstacles we face in relation to our own doubts, fears and anxieties.");  
            printf("(Reversed) Vulnerability, self-doubt, weakness, low self-esteem, lack of confidence, feeling inadequate. And people say I don't have a good mindset!\n");
            }
            printf("Strength ");
            usleep(12000);
            break;
        case 9:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("The Hermit card May I explain this card to you (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("(Upright) The Hermit Tarot card in an upright position generally indicates that you are entering a period of soul searching, self-reflection and spiritual enlightenment.  The Hermit can also indicate isolating yourself or withdrawing into oneself in order to recover from a difficult situation.");
            printf("(Reversed) Loneliness, paranoia, isolation, bring reclusive, withdrawal, anti-social, restrictive, paralysed by fear.\n");
            }
            printf("The Hermit ");
            usleep(9000);
            break;
        case 10:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Additional information about the Wheel of Fortune (Y/N)? ");
            scanf(" %c", &choice);
           if (choice == 'y') { 
            printf("(Upright) The Wheel of Fortune is generally an indicator of luck or destiny and it is a Major Arcana signifier of change. When the Wheel of Fortune appears upright in your Tarot reading big changes are coming. These changes should be for your greater good, but remember not all change is easy even if it’s leading you to your destiny! This trump card is also a karma card so remember to be nice to people on your way up, you may be meeting them again on your way back down!");
            printf("(Reversed) In a career context, the Wheel of Fortune reversed signifies a career becoming stagnant or a period of disruption and uncertainty in your career. Again, this Major Arcana card in reverse can show unwelcome changes coming. You must assess the decisions you’ve made about your career and honesty ask yourself if they have been the right choices to help you achieve what you want to get out of life. Have you seized the opportunities that were available to you? If not, don’t beat yourself up over it.\n"); 
            }
            printf("Wheel of Fortune ");
            usleep(9000);
            break;
        case 11:
        printf(YELLOW_TEXT);
        printf("fortune teller:");
        printf(RESET_TEXT);
        printf("May I go into more detail about the card Justice (Y/N)? ");
        scanf(" %c", &choice);
        if (choice == 'y') {
        printf("I hate this card Just us ? - Justice, karmic justice, consequences, legal disputes, law, truth, honesty, integrity, cause and effect, life lessons.");
        printf("(Upright) The Justice Tarot card relates to karmic justice, legal matters and cause and effect. In a general context Justice is showing you that all actions have consequences."); 
        printf("Justice reversed can indicate injustice or avoidance of karmic justice. This may take the form of being treated unjustly in your life or situation where you are being unfairly affected by the choices or actions of others. You may feel you are being victimised or blamed for something that is not your fault. Whatever the situation, you must try to maintain your balance. Even if you didn’t create the situation, you can choose how you react to it and that can be a lesson in itself.\n"); 
        }
            printf("Justice ");
            usleep(10000);
            break;
         case 12:
        printf(YELLOW_TEXT);
        printf("fortune teller:");
        printf(RESET_TEXT);
        printf("The hanged man card may I go into more detail about this (Y/N)? ");
        scanf(" %c", &choice);
        if (choice == 'y') {
        printf("The Hanged Man Upright Tarot card indicates that you are in a situation that you are not happy with. You may be feeling like you are stuck in a rut or trapped in a situation or frame of mind that is not making you happy but you have the power to release yourself. This may involve walking away from the situation or simply changing your perspective on it. The Hanged Man may also signify that you may be facing a dilemma and are unsure of what path to take.");
        printf("The Hanged Man reversed can indicate that you are being impulsive and making rash decisions as a way to distract yourself from some sort of inner discontentment. This Major Arcana trump card generally shows that you may be jumping from one bad situation to the next without considering where this behaviour is taking you. Ask yourself if there are feelings you trying to avoid confronting or changes that you know need to be made but are reluctant to tackle.\n");  
        }
            printf("The Hanged Man ");
            usleep(8000);
            break;
        case 13:
            printf("Although it is the Tarot card that many people fear, Death generally does not mean physical death. Okay so that's out of the way.");  
            printf("(Reversed) In a general context, Death reversed indicates that you are resisting a change that is necessary for you to move forward. Nothing new can begin while you are holding on to this old negative energy. Death indicates that while you are finding it difficult to let go, once you do, new energy will come into your life to give you a new brighter beginning.\n");
            printf("Death ");
            usleep(9000);
            break;
        case 14:
           printf("(Upright) the Temperance Tarot card signifies balance, peace, patience and moderation.(Reversed) In a general context, Temperance reversed indicates imbalance or overindulgence. This Major Arcana card can be an indicator that you are behaving in a hasty or reckless fashion. It can be a signifier of many issues with excessive or harmful indulgences such as drinking, drug use, gambling, overeating, shopping and the list goes on.\n");
            printf("Temperance ");
            usleep(10011);
            break;
        case 15:
        printf("The Devil Tarot card can signify depression or addiction. It can also be a sign of feeling trapped or restricted. With this Major Arcana card in your Tarot spread you may feel as though outside influences or forces beyond your control are restricting you, leaving you feeling powerless and victimised. However, this is the illusion The Devil creates. You are in control of your own destiny and are not bound by anything other than your own attitudes and behaviour. Don’t give up and don’t give away your power.");
        printf("The Devil reversed can signify that you are becoming aware of the things that have been trapping you and the role you play in allowing them to. It is a great card for anyone battling an addiction or mental illness or engaging in harmful behaviour as it indicates that you are starting to see the light and take back control of yourself and your life. You want to be free from these problems and you are starting to get the motivation to change your circumstances. It can also indicate that you are getting a new perspective on issues that you felt powerless to change previously. The changes you want to make may not be easy but they are necessary for you if you want to have a happy life in the future and you are beginning to realise what you have to do.\n");  
            printf("The Devil ");
            usleep(10011);
            break;
        case 16:
        printf("This card holds Chaos, destruction, sudden upheaval, trauma, unexpected change, disaster, loss, tragedy, revelations, confusion, pain, divorce, abuse, violence, bankruptcy, natural disasters.");
        printf("Whereas the Death card is usually the card people are terrified of, out of all of the cards in the deck, The Tower is the one you really need to brace yourself for. The Tower Tarot card represents chaos and destruction. It is the Major Arcana card of sudden upheaval and unexpected change. This change usually is scary, life changing and often unavoidable. "); 
            printf("The Tower ");
	    usleep(11000);
            break;
        case 17:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("The Star - do you want my take on an aspect of the card (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("The upside of this Star card indicates hope for the future, inspiration and contentment. With this Major Arcana card in your Tarot spread you will find yourself feeling very positive, motivated and free.");
            }
            printf("The Star ");
	    usleep(10066);
            break;
        case 18:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("The Moon, I can go into more detail about this card (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("The Moon Tarot card in an upright position is that everything is not as it seems. It is also the Major Arcana Tarot card of intuition. The Moons tells you that something about a situation or person in your life is not what it appears to be and you need to trust what your instincts are telling you in order to see past this illusion. ");
            printf("The Moon card reversed can signify releasing fears or negative energy clearing. It can also indicate secrets or lies being exposed. If you have been experiencing fear or anxiety it should begin to subside. If you have lost something, The Moon reversed is a sign you will find it. ");
            }
            printf("The Moon ");
            usleep(10666);
            break;
        case 19:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("The sun, I can show you more details (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("The Sun Tarot card is the Major Arcana card of positivity, optimism, freedom and fun. When The Sun appears in an upright position, things should be going well for you as this card also represents success and enthusiasm.");
            printf(" Sun Tarot card reversed can indicate sadness, depression and pessimism. The Sun reversed does not usually mean that your situation or circumstances are all that bad but that you are feeling pessimistic and focusing on the negative which is affecting your outlook on life. ");
            }
            printf("The Sun ");
            usleep(10000);
            break;
        case 20:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("More details about the Judgement card (Y/N)?");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("The Judgement Tarot card can indicate that you and/or someone you care about are being judged too harshly by others. It can also indicate that you are judging people harshly or making snap judgements yourself.");
            printf("Also Judgement card reversed can signify that you are allowing fear and self-doubt to hold you back from making the decisions necessary to move forward in a positive direction. Now is the time to take action. If you delay you may lose the opportunities available to you. ");
            }
            printf("Judgement ");
            usleep(10000);
            break;
        case 21:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("The World card, I can show you an inconsequential aspect (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("The World is an indicator of a major and inexorable change, of tectonic breadth. This change represents a chance for you to bring about a desirable end to the Old and a good beginning to the New. It is indicative of growing maturity, a sense of inner balance and deeper understanding. It suggests that you may be approaching a more final sense of identity, and the security in the self that comes with age. It also represents the falling away of boundaries, sometimes in the effusive sense of the spiritual, but sometimes in a purely physical sense, indicating travels or journeys in the future.");
            }
            printf("The World ");
            usleep(10111);
            break;
        case 22:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("The Ace of Cups, More information (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("The Ace of Cups signifies the beginning of period of strong emotional health for you. Expect copious joy, happiness, and love to surround you during this time. Existing personal relationships may strengthen, meaningful new ones are likely to form. If marriage is in your future, you are likely to lay the foundations for it during this time.");
            }
            printf("Ace of Cups ");
            usleep(10100);
            break;
        case 23:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("The Tow of Cups, may I elaborate (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("The single resounding theme associated with the II of Cups is partnership. What sort of partnership is less clear, but you are in the past, present, or future, part of a powerful one. This partnership will involve absolute trust and commanding respect. It may, of course, be a particularly emotional love affair, but it could just as easily be a deep and abiding friendship. The exact nature may be revealed by the rest of the spread, or it may not. It will be a source of happiness for you, though, a presence in your life you can count on at every turn, and whose well-being will be as important to you as your own.");
            }
            printf("Two of Cups ");
            usleep(11111);
            break;
        case 24:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Three of Cups, More on the meaning (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("This card is both a portent of celebration and cause for it. It tells of great happiness in the future for you, and not of the passing kind, but secure joy which will last and be shared with those around you. Commonly associated with the completion of a meaningful project, or, even more likely, a birth.");
            }
            printf("Three of Cups ");
            usleep(10000);
            break;
        case 25:
            printf("Apathy is the most insidious of evil. If its influence remains unchecked you may lose yourself, and not even realize what you have lost. This card is a strong warning that you must break out of your current cycles. Seek out causes or people to care and fight for. Make new goals, and then seek out new paths to reach them. Now is the time to take risks, as the price of failure is far less than the cost of no longer seeking success.");
            printf("Four of Cups ");
            usleep(10000);
            break;
        case 26:
            printf("This card is a dark omen. Like the rest of its suit, it is bound your emotions, and it brings with it a warning of loss and disappointment. Expect a setback in your life, almost certainly emotional. While this may be emotional, it may be a project or goal you are particularly invested in, emotionally. In certain positions it might also refer to events of the past which continue to cause regret or heartache. The only solution is optimism; this too shall pass.");
            printf("Five of Cups ");
            usleep(900);
            break;
        case 27:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Six of cups, may we cut to the quick here (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("The VI of Cups is rooted deep in the past, but it is also a card closely bound to your happiness. It suggests that your family, your old friends, perhaps even past lovers, are in the process of adding greatly to the joys in your life. This card may also refer to past efforts, labours of love, and personal projects rewarding you, usually in an emotional sense, rather than financial.");
            }
            printf("Six of Cups ");
            usleep(1000);
            break;
        case 28:
           printf(YELLOW_TEXT);
           printf("fortune teller:");
           printf(RESET_TEXT);
           printf("Seven of cups, Life is full of choices. More details (Y/N)? ");
           scanf(" %c", &choice);
           if (choice == 'y') {
           printf("The Seven of Cups Tarot card represents having lots of options to choose from or multiple possibilities open to you. ");
           }
            printf("Seven of Cups ");
            usleep(4000);
            break;
        case 29:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Eight of cups - more details on this card (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("Yes the Eight of Cups represents abandonment. It can signify walking away from people or situations in your life or abandoning your plans.  ");
            }
            printf("Eight of Cups ");
            usleep(8000);
            break;
        case 30:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Nine of Cups, more details on this card (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("Hum.. Nine of Cups Tarot card is a positive card which indicates your wishes will be coming true or your dreams will become a reality. ");
            }
            printf("Nine of Cups ");
            usleep(100);
            break;
        case 31:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Ten of Cups, more info (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("So the Ten of Cups Tarot card is a great card to have in your Tarot reading as it represents true happiness and emotional and spiritual fulfilment. It tells you that you will be living your happy ever after and can look forward to domestic bliss. ");
            }
            printf("Ten of Cups ");
            usleep(100);
            break;
        case 32:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Page of Cups, check your mail (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("You got mail! - Page of Cups is a bringer of messages. This can be in the form of happy news, important information, invitations to social events, gossip or the potential for romantic proposals. It can also represent your inner child so don’t take things too seriously when this card appears. It may be a sign that you need to connect to your inner child by embracing the fun and frivolous side of life.");
            }
            printf("Page of Cups ");
            usleep(10000);
            break;
        case 33:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Knight of Cups, would you like to know more (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("Sorry for your bad luck! -");
            printf("Knight of Cups Tarot card upright can represent proposals, offers, good news and invitations. The news or offers he brings usually carry with them a lot of excitement.");
            printf("The Knight of Cups reversed Tarot card can represent revoked offers or proposals, bad news and withdrawn invitations. The news this Minor Arcana brings usually brings with it heartbreak, sorrow or disappointment. ");
            }
            printf("Knight of Cups ");
            usleep(100);
            break;
        case 34:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Queen of Cups, like to know more about this card (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("If the card is (Upright) the Queen of Cups Tarot card can generally signify a woman or women in your life who will be supportive and caring towards you. ");
            printf("The Queen of Cups reversed Tarot card generally indicates emotional immaturity. She can also indicate that you may be feeling overly sensitive, insecure, depressed or sulky. This Minor Arcana card reversed warns you to avoid becoming bitter or vengeful if things do not go your way. ");
            }
            printf("Queen of Cups ");
	    usleep(10111);
            break;
        case 35:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("King of Cups, can I help you find meaning in the card (Y/N)? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("If upright the King of Cups Tarot card represents kindness, compassion and wisdom. This Minor Arcana card can indicate that you will be finding the balance between your mind and your heart.");
            printf("King of Cups reversed can represent being overly emotional or too sensitive. It can signify a lack of emotional maturity. It can be a sign that you are lacking back bone or are allowing yourself to be too gullible or putting yourself in a position where others can take advantage of you.");
            }
            printf("King of Cups ");
	    usleep(10222);
            break;
        case 36:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("The ");
            printf("Ace of Pentacles, Would you like to know more (Y/N)?");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("Upright the Ace of pentacles represents new beginnings and prosperity. ");
            printf("So Ace of Pentacles reversed represents missed opportunities or lack of opportunities.  ");
            }
            printf("Ace of Pentacles ");
	    usleep(11111);
            break;
        case 37:
            printf("The Two of Pentacles can indicate that you are trying to find or maintain the balance between various areas of your life.");
            printf("Two of Pentacles ");
            usleep(10222);
            break;
        case 38:
            printf("Three of Pentacles is a positive card to get in a Tarot spread. It represents learning, studying and apprenticeship. ");
            printf("Three of Pentacles ");
            usleep(10000);
            break;
        case 39:
            printf("Four of Pentacles Tarot card can indicate that you are holding on to people, possessions, situations or past issues.");
            printf("Four of Pentacles ");
            usleep(10000);
            break;
        case 40:
            printf("Temporary financial hardship, negative change in circumstances, feeling left out in the cold, financial loss, recession, feeling the world is against you, adversity, struggle, hardship, bad luck, homelessness, poverty, unemployment, financial ruin, bankruptcy, outcast, alienation, illness, divorce, breakups, scandal, disgrace... Such is life I guess! unluck.");
            printf("Five of Pentacles ");
            usleep(10000);
            break;
        case 41:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Okay, the six of pentacles would you like to Know More? ");
            scanf(" %c", &choice);
            if (choice == 'y') {
            printf("If Upright the Six of Pentacles Tarot card represents gifts, kindness and generosity. Someone in your life may be very generous towards you with gifts or money or simply generous with their time, knowledge or wisdom. Alternately, you may have so much wealth and prosperity that you are the one in a position to help others. ");
            printf("Also if Reversed - Lack/abuse of generosity, abuse of power or position, gifts with strings attached, subservience, inequality, lack of charity, scams, fake charity, extortion, unemployment, lack of investment, underpaid, undervalued, bad debts, poor financial decisions, greed, meanness, gullibility, kiss-ass... ");
            }
            printf("Six of Pentacles ");
            usleep(10111);
            break;
        case 42:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("Things are coming to fruition, hard work is paying off!\n");
            printf("Seven of Pentacles ");
            usleep(10000);
            break;
        case 43:
            printf(YELLOW_TEXT);
            printf("fortunet teller:");
            printf(RESET_TEXT);
            printf("This card indicates a time of hard work, commitment, diligence and dedication.");
            printf("Tarot card reversed can indicate laziness, idleness, carelessness, lack of effort or poor concentration.");
            printf("Eight of Pentacles ");
            usleep(10111);
            break;
        case 44:             // start our games within a game.
          time(&current_time);
          time_info = localtime(&current_time);
          printf(YELLOW_TEXT);
          printf("fortune teller:");
          printf(RESET_TEXT);
          if (time_info->tm_hour == 12 && time_info->tm_min == 0) {
           printf("It's 12 noon! Time for a coffee break. Would you like to take a coffee break? (Y/N): ");
           scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y') {
            printf("Great! coffee break.\n");
            usleep(26000);
        } else if (choice == 'N' || choice == 'n') {
            printf("Okay, let's continue with the reading then!\n");
        } else {
            printf("Sorry man; Please enter Y or N.\n");
        }
        } else {
        printf("Well you are lucky if it's an Upright card - independence, success, financial stability/security...\n");
        printf("Reversed is A Lack of independence/stability/security, reckless spending, shady investments,,,\n");
        }
            printf("Nine of Pentacles ");
            usleep(10122);
            break;
        case 45:
        printf(YELLOW_TEXT);
        printf("fortune teller:");
        printf(RESET_TEXT);
        char *sayings[] = {
            "Nice to meet you!",
            "Pleasure to meet you!",
            "Lovely to meet you!",
            "Great to meet you!",
            "Delighted to meet you!",
            "Wonderful to meet you!",
            "Delighted to meet like this!"
        };
        srand(time(NULL));
        int random_index = rand() % (sizeof(sayings) / sizeof(sayings[0]));
        printf("My name is morning star the fotune teller %s\n", sayings[random_index]);

	    printf("Ten of Pentacles ");
            usleep(10123);
            break;
        case 46:
            busted();  // busted
            printf("Page of Pentacles ");
            usleep(11002);
            break;
        case 47:
            random_fortune();
            printf("Knight of Pentacles ");
            usleep(10123);
            break;
        case 48:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            printf("But do not mistake her for being only a homebody - alongside all these motherly attributes, she can plan business ventures and execute her plans successfully.");
            printf("If the card is Upright - When it comes to all things career oriented, the Queen of Pentacles tarot card can bring great success. She is a talented businesswoman who is practical, organized and very capable in anything she puts her mind to. ");
            printf("if the card is Reversed -Her focus on the material may manifest negatively, making her greedy, and yet lacking in her sense of duty and capability.\n");
            printf("Queen of Pentacles ");
            usleep(31);
            break;
        case 49:
            printf("King of Pentacles ");
            usleep(100);
            break;
        case 50:
            random_fortune();
            printf("Ace of Swords ");
            usleep(99);
            break;
        case 51:
            printf("Two of Swords ");
            usleep(199);
            break;
        case 52:
            printf("Three of Swords ");
            usleep(10099);
            break;
        case 53:
            printf("Four of Swords ");
            usleep(1);
            break;
        case 54:
            printf("Five of Swords ");
            usleep(10200);
            break;
        case 55:
            printf("Six of Swords ");
            usleep(300);
            break;
        case 56:
            printf("Seven of Swords ");
            usleep(900);
            break;
        case 57:
            printf("Eight of Swords ");
            usleep(10000);
            break;
        case 58:
            printf("Nine of Swords ");
            usleep(999);
            break;
        case 59:
            printf("Ten of Swords ");
            usleep(1000);
            break;
        case 60:
            printf("Page of Swords ");
            usleep(10000);
            break;
        case 61:
            printf("Knight of Swords ");
            usleep(77);
            break;
        case 62:
            printf("Queen of Swords ");
            usleep(389);
            break;
        case 63:
            printf("King of Swords ");
            usleep(997);
            break;
        case 64:
            printf("Ace of Wands ");
            usleep(45);
            break;
        case 65:
            printf("Two of Wands ");
            usleep(33);
            break;
        case 66:
            printf("Three of Wands ");
            usleep(10033);
            break;
        case 67:
            printf("Four of Wands ");
            usleep(10118);
            break;
        case 68:
            printf("Five of Wands ");
            usleep(10444);
            break;
        case 69:
            printf("Six of Wands ");
            usleep(10112);
            break;
        case 70:
            printf("Seven of Wands ");
            usleep(10223);
            break;
        case 71:
            printf("Eight of Wands ");
            usleep(12013);
            break;
        case 72:
            printf("Nine of Wands ");
            usleep(10100);
            break;
        case 73:
            printf("Ten of Wands ");
            usleep(10101);
            break;
        case 74:
            printf("Page of Wands ");
            usleep(10000);
            break;
        case 75:
            printf("Knight of Wands ");
            usleep(1888);
            break;
        case 76:
            printf("Queen of Wands ");
            usleep(389);
            break;
        case 77:
            printf(YELLOW_TEXT);
            printf("fortune teller:");
            printf(RESET_TEXT);
            greetBasedOnTime();
            printf("King of Wands ");
            usleep(11);
            break;
        default:
            printf(" ");
    }
}

void centerText(const char *text) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int width = w.ws_col;
    int len = strlen(text);

    int padding = (width - len) / 2;
    for (int i = 0; i < padding; i++) {
        putchar(' ');
        fflush(stdout);
        usleep(10000); // 10 milliseconds delay
    }
    printf("%s\n", text);
}

void introText(char *text) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Get terminal width

    int terminal_width = w.ws_col;
    int text_length = strlen(text);
    int padding = (terminal_width - text_length) / 2;

    for (int i = 0; i < padding; i++) {
        putchar(' ');
        fflush(stdout);
        usleep(57000);
    }
    printf(TEST_TEXT);
    printf("%s\n\n", text);
    printf(RESET_TEXT);
    usleep(57000);
}

int main() {
    clearScreen();
    char intro_choice;
    introText("Skip intro text and get playing? ");
    printf("(Y/N)? ");
    scanf("%c", &intro_choice);
    if (intro_choice == 'n') {
    introText("[ Mystic Insights: The Fortune Telling Game ]");
    introText("Welcome, seeker, to 'Mystic Insights,' where your fate awaits to be unveiled through the ancient art of fortune telling.");
    introText("In this enchanting game, you will embark on a journey of self-discovery and revelation. Through a series of intriguing divination methods and mystical rituals, the secrets of your past, present, and future will be unveiled before your very eyes. ");
    introText("Drawn from the wisdom of ages past, 'Mystic Insights' offers you the opportunity to peer beyond the mundane and glimpse the threads of fate that weave the tapestry of your life. Whether you seek answers to burning questions, guidance through life's labyrinthine paths, or simply a glimpse into the unknown, our game promises to ignite the flames of curiosity within your soul.");
    introText("Are you ready to unlock the mysteries that lie hidden in the depths of your being? Prepare to delve into the realms of tarot cards, crystal balls, astrology charts, and more. Each step you take will bring you closer to the profound truths that shape your existence.");
    introText("But remember, dear traveler, the journey itself is as important as the destination. Embrace the unknown with an open heart and a courageous spirit, for it is through the exploration of the unseen that we truly discover ourselves.");
    introText("So, gather your courage, cast aside your doubts, and let the winds of destiny guide you. Welcome to 'Mystic Insights: The Fortune Telling Game' — where the answers you seek await within the whispers of the cosmos.");
    introText("Beware the fortune teller, dear reader, and let not the allure of mystery cloud your judgment. May your quest for insight be guided by discernment and tempered by wisdom, for only then can you navigate the labyrinth of fate with clarity and grace.");
    int total_seconds = 1.2 * 60 + 5; // 1 minute countdown.
    int remaining_seconds;

    for (remaining_seconds = total_seconds; remaining_seconds > 0; remaining_seconds--) {
        printf("\0339");
        printf(" pause time remaining: %d seconds    \r", remaining_seconds);
        fflush(stdout); // Flush the output buffer to ensure immediate display
        sleep(1); // Pause for 1 second
    }

    printf("Time's up! Resuming...\n");
    usleep(189);

    } else if (intro_choice == 'y') {
    usleep(140000);
    }
    // Combine current time with process ID and system clock for better seeding
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)getpid() ^ (unsigned int)clock();
    srand(seed);
    clearScreen();
    printf(GRAY_TEXT);
    centerText("Imagine walking down a dimly lit alleyway...");
    centerText("You come across a mysterious figure sitting at a small table,");
    centerText("surrounded by the hustle and bustle of the city.");
    centerText("Liquor bottles of all shapes and sizes are scattered around him,");
    centerText("each holding its own story.");
    centerText("Street trash litters the ground, but amidst the chaos,");
    centerText("there's an aura of mystique.");
    centerText("The fortune teller looks up, his eyes gleaming devillish intent.");
    centerText("He beckons you closer, inviting you to uncover the secrets");
    centerText("that lie beyond the veil of reality.");
    centerText("His sales pitch for your loose pocket change sterling.");
    centerText("But beware, for in this realm of shifting sands and hidden truths, ");
    centerText("every choice you make may alter the course of your journey.");
    printf(RESET_TEXT);
    printf("\nWould you like to engage with the fortune teller? (Yes/No): ");
    char response[10];
    scanf("%s", response);
     for (int i = 0; response[i]; i++) {
     if (isalpha(response[i])) {
            response[i] = tolower(response[i]);
        }
     }
     if (strcmp(response, "y") == 0) {
        strcat(response, "es");
     }
     if (strcmp(response, "yes") == 0)  {
     clearScreen();
     printf(RED_TEXT);
     centerText("You've decided to engage with the fortune teller.");
     printf(RESET_TEXT);
    int drawnCards[TOTAL_CARDS];
    int numCardsDrawn = 0;
    printf(YELLOW_TEXT);
    printf("The initial cards drawn:\n", RESET_TEXT);
    printf(RESET_TEXT);
    for (int i = 0; i < INITIAL_DRAW; i++) {
        int card = rand() % TOTAL_CARDS;
        drawnCards[i] = card;
        displayCard(card);
        numCardsDrawn++;
        determineCardState();
    }

    char response;
    do {
        printf(RED_TEXT);
        printf("Do you want to draw more cards? (y/n): ");
        printf(RESET_TEXT);
        scanf(" %c", &response);
        if (response == 'y') {
            int card = rand() % TOTAL_CARDS;
            drawnCards[numCardsDrawn] = card;
            printf(RED_TEXT);
            printf("Additional card drawn:\n");
            printf(RESET_TEXT);
            displayCard(card);
            determineCardState();
            numCardsDrawn++;
        }
    } while (response == 'y');
    srand(seed/MAX_CARD_VALUE);
    time_t current_time;
    struct tm *time_info;
    time(&current_time);
    time_info = localtime(&current_time);
    if (time_info->tm_hour == 0 && time_info->tm_min == 0) {
        printf("It's midnight! The dealer drops random cards:\n");
        int luckyCard = rand() % MAX_CARD_VALUE + 1;
        for (int i = 0; i < luckyCard; i++) {
            int card = rand() % TOTAL_CARDS;
            displayCard(card);
        }
        determineCardState();
    }
    usleep(15000*2);
    clearScreen();
    initializeRandom();
    int randomIndex = rand() % MUM_MESSAGES;
    printf(YELLOW_TEXT);
    printf("fortune teller:");
    printf(RESET_TEXT);
    printf(TEST_TEXT);
    printf("%s\n", goodbyeMessages[randomIndex]);
    printf(RESET_TEXT);
    initializeRandom();
    int ending = rand() % 20;
    // Different possible endings
    switch (ending) {
        case 0:
            centerText("The fortune teller predicts that you will come into great wealth and live a life of luxury.\n");
            break;
        case 1:
            centerText("The fortune teller warns you,"); 
            centerText("'Beware! Danger lurks around every corner. Trust no one in this dark alley.'\n");
            break;
        case 2:
            centerText("With a serene smile, the fortune teller  shares,");
            centerText(" 'Love will find its way into your heart soon.'\n");
            break;
        case 3:
            centerText("The fortune teller foretells,");
            centerText(" 'Your journey will take you to far-off lands and distant shores.'\n");
            break;
        case 4:
            centerText("You are told, 'Your kindness will be rewarded tenfold.'\n");
            break;
        case 5:
            centerText("Darkness will cloud your path, but the fortune teller reassures,");
            centerText(" 'Light will guide you through.'\n");
            break;
        case 6:
            centerText("Fortune teller whispers,");
            centerText(" 'You will uncover a long-hidden secret that will change everything here. Come back soon!'\n");
            break;
        case 7:
            centerText("The wise fortune teller advises,");
            centerText(" 'Take heed of those who offer false promises, for they lead to ruin.'\n");
            break;
        case 8:
            centerText("Your creative talents will lead you to great success,");
            centerText(" Sorry your reveal is not a winner.\n");
            break;
        case 9:
            centerText("In a soothing voice, the fortune teller says,");
            centerText(" 'Forgiveness will bring you peace and liberation.'\n");
            break;
        case 10:
            centerText("The fortune teller smiles with sharp white teeth,");
            centerText(" 'A new friend will enter your life and bring you joy sooner then you think...'\n");
            break;
        case 11:
            centerText("Your past mistakes will teach you valuable lessons for the future,");
            centerText(" I predict passing soon.\n");
            break;
        case 12:
            centerText("The fortune teller nods knowingly,");
            centerText(" 'A journey of a thousand miles begins with a single step.'\n");
            break;
        case 13:
            centerText("An unexpected encounter will change the course of your destiny today,");
            centerText(" says I mysteriously. Booo!\n");
            break;
        case 14:
            centerText("With certainty, the forutne teller declares,");
            centerText(" 'Your inner strength will carry you through the toughest of times.'\n");
            break;
        case 15:
            printf("Suddenly, the fortune teller's eyes turn blood-red, and it lets out a chilling scream...\n");
            printf("You watch in horror as fortune teller transforms into a rat and scurries down a nearby storm drain, vanishing into the darkness.\n");
            break;
        case 16:
            printf("The cards burst into fire ominously, and the fortune teller's voice grows distant...\n");
            printf("You realize too late that you're trapped in her alley, doomed to wander its shadows forever.\n");
            break;
        case 17:
            printf("A bone-chilling wind sweeps through the alley, extinguishing all light...\n");
            printf("When the darkness recedes, you find yourself alone, with nothing but the echo of the fortune teller's laughter.\n");
            break;
        case 18:
            printf("As you lean in to hear her prediction, you catch a glimpse of something monstrous lurking behind her...\n");
            printf("Before you can react, the alley fades away, and you wake up, drenched in cold sweat, safe but shaken.\n");
            break;
        case 19:
            printf("The fortune teller's smile twists into a sinister grin, his voice drips with malice...\n");
            printf("Before you can blink, you're trapped in a nightmare, and the fortune teller's words become your eternal torment.\n");
            break;
        default:
            centerText("The fortune teller seems to be lost in thought,");
            centerText(" and the cards remains silent.\n");
            break;
    }
    return 0;
    } else {
    clearScreen();
    printf(RED_TEXT);
    centerText("You've decided to ignore the fortune teller and continue about your day.");
    printf(RESET_TEXT);
    int randomExit = rand() % 5 + 1;
    switch (randomExit) {
            case 1:
                centerText("You walk away, feeling uncertain about what could have been.");
                centerText("With each step, the darkness of the alley seems to swallow the echoes of your hurried departure.\n");
                break;
            case 2:
                centerText("You decide to continue on your journey checking the time, leaving the alley behind.\n");
                centerText("Turning your back to the alley, you sense a chill in the air, whispering of forgotten tales.\n");
                break;
            case 3:
                centerText("As you turn to leave, you feel a sense of relief wash over you.\n");
                centerText("As you step away, you notice something odd on the ground.");
                centerText("A marker lies there, its inscription sending shivers down your spine:\n");
                centerText("On July 7th @ midnight a homeless mans body found  here.");
                centerText("The cryptic message fills you with a sense of foreboding...");
                break;
            case 4:
                centerText("You walk away, feeling the weight of unseen eyes lingering on your back.");
                break;
            case 5:
                centerText("As you move further away, you notice something unsettling...");
                centerText("A faded photograph lies on the ground, capturing moments of joy, yet the fortune teller's face remains obscured.");
                centerText("When you glance back, you realize the fortune teller's visage lacks any facial details.");
                break;
}
    printf("\n");
    // Introduce a chance for an alternative ending (1 in 3 chance)
        int alternativeEndingChance = rand() % 3;
        int twistEnding = (alternativeEndingChance == 0) ? 1 : 0;
       // Add a twist ending for some stories
        if (twistEnding) {
            centerText("However, as you glance back one last time, you are startled to see the alley completely empty, devoid of any trace of the fortune teller.\n");
            centerText("The unsettling feeling lingers as you continue on your way, questioning the reality of your brief encounter.\n");
        } else {
            centerText("When you steal another look, the fortune teller's face is a featureless void, sending shivers down your spine.\n");
        }

    return 0;
    }
}
