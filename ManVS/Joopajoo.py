# MAN VS THE ORCS!!!!!
# By Johannes Kalliokoski 274539
#Skaalautuvaa tähtäsin.
# If u find bugs let me know, known ones are:
# highscore tables spaces, flying by spamming jump(not gona fix its fun),
# Random death if spamming hit, the arm might also fly away from the body and
# Errors if closing the game by x on top right when th game is running,
# (not if on menu) This is because the threads are not stopped correctly. just dont.
# Whether action is a function or a method is a bit weirdly decided here. I admit.

"""
Games instructions can be read by pressing the help button in main menu.

"""

from tkinter import *
import tkinter.font as font
import random
import time
import threading

class stoppistaThread(threading.Thread):
    #simple thread-class, that makes me able to stop the threads nicely.
    def __init__(self, *args, **kwargs):
        super(stoppistaThread, self).__init__(*args, **kwargs)
        self._stoppista_event = threading.Event()
    def stop(self):
        self._stoppista_event.set()
    def stopped(self):
        return self._stoppista_event.is_set()


class Game:
    #Main class of the game. Ill explain each part separately.
    def __init__(self):
        #Constructor
        self.highscore = {} #used to store scores of players
        self.scores = [] #Also used to store scores of players in different form
        self.P = 0 #Points of player
        self.__window = Tk()
        self.__window.title("ManVS")
        self.__window.geometry("1000x500")
        self.__window.resizable(0, 0)
        self.name = StringVar() #Name of player
        self.name = "Name" #Standard name, if no new name is set
        font.nametofont("TkDefaultFont").configure(size=15) #I wanted big font
        self.color = "black" #Color of player, standard is black
        self.alkunaytto()

    def alkunaytto(self):
        #Starting screen/menu
        #instructions opens help window
        self.bgg = PhotoImage(file="tausta1.gif")
        self.bgg1 = Label(self.__window, image=self.bgg, width=1000, height=500)
        self.bgg1.pack()
        self.__instructions = Button(self.__window, text="Help", bg="lightgrey",
                                     activebackground="grey", width=10, height=2,
                                     command=self.help)
        self.__instructions.place(relx=1, rely=0, anchor=N+E)
        #starts the game
        self.__start_btn = Button(self.__window, text="START", bg="lightblue",
                                  activebackground="blue", width=15, height=5,
                                  command=self.start_game)
        self.__start_btn.place(relx=0.5, rely=0.5, anchor=CENTER)
        #Shows highest scores
        self.high_score = Label(self.__window, text="Highest scores",
                                bg="lightblue")
        self.high_score.place(relx=0, rely=0, anchor=N + W)
        #Opens character creation -window
        self.create_char = Button(self.__window, text="Create your character",
                                  command=self.create)
        self.create_char.place(relx=0.1, rely=0.5, anchor=N)

        #Following is compiling the highscore table:
        if len(self.scores) == 0:
            self.scores_txt = "Empty" #If no scores are set
        else:
            self.scores_txt = ""
            highscore = sorted(self.scores, key= lambda x: x[0], reverse=True)
            #I want it to show top 5:
            if len(self.scores) <= 5:
                amount = len(self.scores)
            else:
                amount = 5
            for j in range(amount):
                self.scores_txt = self.scores_txt + highscore[j][1]
                self.scores_txt.strip(" ")
                self.scores_txt ="{}\n".format(self.scores_txt)

        self.scorefield = Label(self.__window, text=self.scores_txt,
                                bg="lightblue")
        self.scorefield.place(relx=0, rely=0.1, anchor=NW)

    def create(self):
        #Character creation window
        self.__create = Tk()
        self.__create.title("Character creation")
        self.__create.geometry("500x500")
        self.__create.resizable(0, 0)
        self.__color_txt = Label(self.__create, text="Choose your color:",
                                 font=("TkDefaultFont", 20))
        self.__color_txt.place(relx=0, rely=0, anchor=NW)
        #Here command is color() function tht colors the character.
        #Had to use lambdas to be able to put arguments for the function.
        self.__color_button1 = Button(self.__create, bg="black", width=2,
                                      height=2, command= lambda: color(self, 1))
        self.__color_button1.place(relx=0.1, rely=0.2, anchor=W)
        self.__color_button2 = Button(self.__create, bg="yellow", width=2,
                                      height=2, command= lambda: color(self, 2))
        self.__color_button2.place(relx=0.3, rely=0.2, anchor=W)
        self.__color_button3 = Button(self.__create, bg="white", width=2,
                                      height=2, command= lambda: color(self, 3))
        self.__color_button3.place(relx=0.5, rely=0.2, anchor=W)
        self.name_txt = Label(self.__create, text="Name:", font=("TkDefaultFont",
                                                                 20))
        self.name_txt.place(relx=0, rely=0.4, anchor=W)
        self.name_field = Entry(self.__create)
        self.name_field.place(relx=0, rely=0.6, anchor=W)
        #Here command is lambda function tht updates the name
        self.name_set_btn = Button(self.__create, text="Set", command = lambda:
                                   self.set_name(self.name_field.get()))
        self.name_set_btn.place(relx=0.3, rely=0.6, anchor=W)

    def set_name(self, name):
        #Simple name changer.
        self.name = name

    def help(self):
        #Help window
        txt = "The objective of this game is to get as many points as you can.\n" \
              "You get points by killing orcs and passing cacti. \n" \
              "To jump over cactus, press the jump button, or spacebar. \n" \
              "To hit an orc, press the hit button or h in your keyboard. \n" \
              "Your healthpoints and score are shown on the top left. \n" \
              "Yellow boxes are bonuses, an sword or a helm. \n" \
              "Sword raises your dmg by 1, and degrades after hitting an orc \n"\
              "Helm gives you 1 bonus hp, and degrades after taking dmg. \n"\
              "Harakiri -button ends the game. \n" \
              "Highscore players best scores (Top 5) \n" \
              "Hope you enjoy playing as much as I enjoyed making this ! :D"

        self.__help = Tk()
        self.__help.title("Help")
        self.__help_txt = Label(self.__help, text=txt)
        self.__help_txt.pack()

    def start_game(self):
        #Starts the game!
        self.bg2 = PhotoImage(file="tausta[826].gif")
        self.gg2 = PhotoImage(file="ground.gif")
        self.P = 0
        #qwe is my choice for canvas name :D
        self.qwe = Canvas(self.__window, width=1000, height=500, bg="grey")
        self.qwe.pack()
        #Cool .gifs for ground and background:
        self.qwe.create_image(0,0, image=self.bg2, anchor=NW)
        self.qwe.create_image(0,500, image=self.gg2, anchor=S, tags="ground")
        self.qwe.create_image(500, 500, image=self.gg2, anchor=S, tags="ground")
        """
        Destroy the now useless stuff and add some new buttons:
        Threader() is thread starter
        """
        self.__start_btn.destroy()
        self.bgg1.destroy()
        self.__jump_btn = Button(self.__window, text="JUMP", bg="lightblue",
                                 width=10, height=3,
                                 command= lambda: self.threader(jumppy))
        self.__jump_btn.place(relx=0, rely=1, anchor=S+W)
        self.__window.bind("<space>", lambda event: self.threader(jumppy))
        self.hit_btn = Button(self.__window, text="HIT", bg="red",
                                 width=10, height=3, fg="white",
                              command= lambda: self.threader(hitty))
        self.hit_btn.place(relx=0.2, rely=1, anchor=S + W)
        self.__window.bind("h",lambda event: self.threader(hitty))
        #Creating the character:
        self.qwe.create_rectangle(300, 350, 200, 200, fill=self.color,
                                  tags="man")
        self.qwe.create_rectangle(320, 260, 200, 240, fill=self.color,
                                    tags="arm", outline=self.color)
        self.qwe.create_rectangle(200, 260, 180, 240, fill=self.color,
                                    tags="man", outline=self.color)
        self.qwe.create_rectangle(280, 230, 270, 220, fill="white",
                                    tags="eye")
        self.qwe.create_rectangle(230, 230, 220, 220, fill="white",
                                    tags="eye")
        self.elkut_txt=Label(self.__window, text="Health")
        self.elkut_txt.place(relx=0, rely=0, anchor=N+W)
        self.qwe.create_rectangle(70, 15, 80, 25, fill="red", tags="hp1")
        self.qwe.create_rectangle(90, 15, 100, 25, fill="red", tags="hp2")
        self.qwe.create_rectangle(110, 15, 120, 25, fill="red", tags="hp3")
        #health and point system:
        self.health = 3
        self.points = IntVar()
        self.points.set(self.P)
        self.score = Label(self.__window, text="Points")
        self.score.place(relx=0, rely=0.1, anchor=W)
        self.score_2 = Label(self.__window, textvariable=self.points)
        self.score_2.place(relx=0.1, rely=0.07, anchor=N + W)
        self.alive = True #status thing thts used later
        self.orc_hp = 2
        self.battle_status = 0 #status thing used to determine dmg to orcs
        self.qwe.update() #make sure everythings on point
        self.t = stoppistaThread(target=obstacles, args=(self,))
        self.t.start() #making obstacles() run by stoppable thread!
        self.difficulty = 0.15 #base difficulty, basicly speed
        #ends the game
        self.stop_btn = Button(self.__window, text="Harakiri", command=self.kys)
        self.stop_btn.place(relx=0, rely=0.2)
        self.bonushp = False
        self.bonusdmg = False
    def kys(self):
        #Game ender
        self.health = 0
        ukkeli_kuolee_epic(self)
        self.alive= False

    def threader(self, target):
        # Jump animation would make obstacles stop moving so I did this
        self.T = stoppistaThread(target=target, args=(self,))
        self.T.start()

    def start(self):
        self.__window.mainloop()

    def boost(self, nro):
        #Toggles powerups
        if nro == 1 and self.bonushp == False :
            self.qwe.create_rectangle(310, 240, 190, 190, tags="rune_med",
                                      fill="lightblue")
            self.qwe.tag_lower("rune_med", "man")
            self.qwe.create_rectangle(130, 15, 140, 25, fill="pink", tags="hpx")
            self.bonushp = True
        elif nro == 3 and self.bonusdmg == False:
            self.qwe.create_rectangle(320, 260, 340, 200, tags="rune_sword",
                                      fill="lightblue")
            self.bonusdmg = True
    def stoppista(self):
        #Ends the game after death. Also saves the score:
        name = str(self.name)
        self.scores.append([self.P, "Name: {}; Points: {}; Color: {}".
                           format(name, self.P, self.color)])
        self.alive= False
        self.battle_status = 0
        try:
            self.T.stop()
        except AttributeError:
            pass
        self.t.stop()
        self.elkut_txt.destroy()
        self.qwe.destroy()
        self.__jump_btn.destroy()
        self.hit_btn.destroy()
        self.stop_btn.destroy()
        self.score.destroy()
        self.score_2.destroy()
        self.alkunaytto()

def hitty(self):
    #Attack animation, also used to dmg orcs if on battle
    if self.bonusdmg == False:
        self.qwe.move("arm", 50, -5)
        self.qwe.update()
        if self.battle_status == 1:
            self.orc_hp = self.orc_hp - 1
            if self.orc_hp == 0:
                orc_death(self)
                self.P += 1
                self.points.set(self.P)
                #difficulty raises when player has enough points:
                if self.P == 5:
                    self.difficulty = 0.1
                elif self.P == 10:
                    self.difficulty = 0.07
                elif self.P == 15:
                    self.difficulty = 0.05
                elif self.P == 20:
                    self.difficulty = 0.03
                self.battle_status = 0
        time.sleep(0.2)
        self.qwe.move("arm", -50, 5)
    elif self.bonusdmg == True:
        #Animation using sword: 320, 260, 340, 200
        self.qwe.coords("rune_sword", 320, 260, 380, 240)
        self.qwe.update()
        if self.battle_status == 1: # same as before, only 2 dmg.
            self.orc_hp = self.orc_hp - 2
            if self.orc_hp == 0:
                orc_death(self)
                self.P += 1
                self.points.set(self.P)
                #difficulty raises when player has enough points:
                if self.P == 5:
                    self.difficulty = 0.1
                elif self.P == 10:
                    self.difficulty = 0.07
                elif self.P == 15:
                    self.difficulty = 0.05
                elif self.P == 20:
                    self.difficulty = 0.03
                self.battle_status = 0
            self.qwe.delete("rune_sword")
            self.bonusdmg = False
        time.sleep(0.2)
        self.qwe.coords("rune_sword", 320, 260, 340, 200)

def jumppy(self):
    #Jump animation
    for i in range(2):
        if self.bonusdmg == True:
            self.qwe.move("rune_sword", 0, -100)
        if self.bonushp == True:
            self.qwe.move("rune_med", 0, -100)
        self.qwe.move("eye", 0, -100)
        self.qwe.move("man", 0, -100)
        self.qwe.move("arm", 0, -100)
        self.qwe.update()
        time.sleep(0.1)
    time.sleep(0.3)
    for i in range(2):
        if self.bonusdmg == True:
            self.qwe.move("rune_sword", 0, 100)
        if self.bonushp == True:
            self.qwe.move("rune_med", 0, 100)
        self.qwe.move("eye", 0, 100)
        self.qwe.move("man", 0, 100)
        self.qwe.move("arm", 0, 100)
        self.qwe.update()
        time.sleep(0.1)

def obstacles(self):
    #spawns obstacles and orcs
    while self.alive:
        nro = random.randint(1,3)
        x = 1000
        time.sleep(1)
        if nro == 1: #obstacles:
            height=random.randint(1,2)
            if height == 1:
                y = 0
            else:
                y = 100

            dmg_taken = False #used to determine if player should get a point
            rect = self.qwe.create_rectangle(x, 350, x-50, 300-y, fill="green",
                                             tags="este")
            while x >= 50:
                x = x - 50
                self.qwe.move("este", -50, 0)
                self.qwe.update()

                #Here is being checked if player hits the obstacle:
                if self.qwe.coords("este") == [250.0, 300.0, 300.0, 350.0] \
                        and self.qwe.coords("man") == [200.0, 200.0, 300.0, 350.0]\
                        and y == 0:
                    take_dmg(self)
                    dmg_taken = True
                elif self.qwe.coords("este") == [250.0, 200.0, 300.0, 350.0] \
                        and self.qwe.coords("man") == [200.0, 200.0, 300.0, 350.0]\
                        and y == 100:
                    take_dmg(self)
                    dmg_taken = True
                elif self.qwe.coords("este") == [250.0, 200.0, 300.0, 350.0] \
                        and self.qwe.coords("man") == [200.0, 100.0, 300.0, 250.0]\
                        and y == 100:
                    take_dmg(self)
                    dmg_taken= True
                else:
                    time.sleep(self.difficulty)
            self.qwe.delete(rect)
            if dmg_taken != True and self.alive == True:
                self.P += 1
                self.points.set(self.P)
                #Again making the game harder if enough points:
                if self.P == 5:
                    self.difficulty = 0.1
                elif self.P == 10:
                    self.difficulty = 0.07
                elif self.P == 15:
                    self.difficulty = 0.05
                elif self.P == 20:
                    self.difficulty = 0.03
        elif nro == 2:
            #spawn orc:
            self.orc_hp = 2
            self.qwe.create_rectangle(1000, 200, 900, 350,
                                             fill="brown", tags="orc")
            self.qwe.create_rectangle(1020, 260, 1000, 240, fill="brown",
                                      tags="orc", outline="brown")
            self.qwe.create_rectangle(880, 260, 1000, 240, fill="brown",
                                      tags="orcarm", outline="brown")
            self.qwe.create_rectangle(990, 240, 980, 230, fill="red",
                                      tags="orc")
            self.qwe.create_rectangle(910, 240, 920, 230, fill="red",
                                      tags="orc")
            while x > 450:
                time.sleep(2*self.difficulty)
                x = x-50
                self.qwe.move("orcarm", -50, 0)
                self.qwe.move("orc", -50, 0)
                self.qwe.update()
            if x == 450:
                monster_battle_epic(self)
        elif nro == 3:
            #Lootcrate:
            #loot_nro = random.randint(1,6)
            loot_nro = random.randint(1,3)
            if loot_nro % 2 == 0:
                pass #no loot
            elif loot_nro == 1 or loot_nro == 3:
                #spawn helmet or sword:
                rect = self.qwe.create_rectangle(x, 350, x - 75, 275,
                                                 fill="yellow",
                                                 tags="crate")
                while x >= 50:
                    x = x - 50
                    self.qwe.move("crate", -50, 0)
                    self.qwe.update()
                    time.sleep(self.difficulty)
                    if x == 400:
                        #No multiples of same powerup:
                        if loot_nro == 1 and self.bonushp == False:
                            self.boost(1)
                            self.qwe.delete("crate")
                        elif loot_nro == 3 and self.bonusdmg == False:
                            self.boost(3)
                            self.qwe.delete("crate")
                self.qwe.delete("crate")

    #if player dies:
    if self.alive == False:
        self.stoppista()
def monster_battle_epic(self):
    #Monsters fight animation:
    self.battle_status = 1
    while self.orc_hp > 0 and self.battle_status == 1:
        time.sleep(7 * self.difficulty)
        self.qwe.move("orcarm", -50, -5)
        self.qwe.update()
        time.sleep(self.difficulty)
        self.qwe.move("orcarm", 50, 5)
        self.qwe.update()
        if self.orc_hp > 0:
            take_dmg(self)
        if self.alive == False:
            time.sleep(1)
            return

def orc_death(self):
    #orc death animation:
    self.qwe.itemconfig("orceye", fill="brown")
    self.qwe.coords("orc", 450, 150, 550, 250)
    self.qwe.update()
    time.sleep(0.3)
    self.qwe.move("orc", 0, 100)
    self.qwe.update()
    self.qwe.delete("orc")
    self.qwe.delete("orcarm")
    return

def take_dmg(self):
    #makes healthbars turn black and kills player if health hits zero:
    hp_current = "hp{}".format(self.health)
    if self.health > 1 and self.bonushp == False:
        self.qwe.itemconfig(hp_current, fill="black")
        self.health = self.health - 1
    elif self.health == 1 and self.bonushp == False:
        self.qwe.itemconfig(hp_current, fill="black")
        ukkeli_kuolee_epic(self)
        self.alive = False
    elif self.bonushp == True:
        self.qwe.delete("hpx")
        self.qwe.delete("rune_med")
        self.bonushp = False
def ukkeli_kuolee_epic(self):
    #death animation:
    try:
        self.T.stop()
    except AttributeError:
        pass
    self.qwe.itemconfig("eye", fill=self.color)
    self.qwe.coords("man", 200, 150, 350, 250)
    time.sleep(0.3)
    self.qwe.move("eye", 0, 100)
    self.qwe.move("man", 0, 100)
    self.qwe.Dedlbl = Label(text="U DIED!", bg="red", fg="white")
    self.qwe.Dedlbl.place(relx=0.5, rely=0.5, anchor=CENTER)

def color(self, nro):
    #Simple coloring function
    if nro == 1:
        self.color = "black"
    elif nro == 2:
        self.color = "yellow"
    else:
        self.color = "white"

def main():
    ui = Game()
    ui.start()
main()

