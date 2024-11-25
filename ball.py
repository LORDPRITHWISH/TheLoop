import time
import math
import threading
# import multiprocessing
from tkinter import *
from tkinter.ttk import Progressbar  

H=700
WI=1200

#        0           1       2       3           4           5           6           7        8        9

mode=['normal','shooting','barrier','life','ballsizeinc','strength','padsizeinc','padsizedec','slow','EXPLODE']
colour=['yellow','green','pink','hotpink',  '',             '',         '',         '',        '',      'CYAN']

def glodec():
    global HS,WS,PH,PS,DEG,NOLI,OSPEED,BEGIN,DRP,LASE,LAON
    global MD,SS,POS,DRL,TAR,MW,PA,OSPEED,SCORE,OUT,LIIN
    gloreset()
    SCORE=0

    HS=0

    WS=0

    PW=60
    PH=PW//3 #PAD HEIGHT
    PS=20

    OSPEED=SPEED

    DEG=70      # 70
    MD=20   #is the distance
        
    SS=20   # SQUARE SIZE

    NOLI=0

    POS=[]  #box location
    DRL=[]   #drop  location
    TAR=[]  #TARGET
    LIIN=[]
    LASE=[]
    # I=0

    MW=False   # mouse on off
    OUT=True
    BEGIN=True
    DRP=False
    LAON=False

def gloreset():
    global PW,BAR,SH,DIV,NS,DEF,MAXL,BULLMOVE,SP,SPEED,BALLCOL,PB,PBB,PA,AIM
    PW=60
    SPEED=4
    BAR=False   # barrier
    # DIV=True    # ball will be diverted on hit
    SP=50  #BALL SIZE
    SH=False    # if shooting is on
    DEF=True    # deflict the ball on hit
    NS=SP       # big ball
    BULLMOVE=False
    PBB=False  # Progressbar is present and running
    
    PA=False    # pause
    MAXL=8   # MAX LIFE
    BALLCOL='ORANGE'
    PB=[]
    AIM=TRUE

def block(p):
    global c,POS
    nc=c.create_rectangle(p[0]*SS,p[1]*SS,(p[0]+p[2])*SS,(p[1]+p[3])*SS,fill=colour[p[4]],tags=mode[p[4]])
    c.addtag_withtag('obsticle',nc)
    POS.append(nc)
    
def blc():
    k=[]
    nc=WI//SS
    nr=400//SS
    #####          k format (colum,row,side x,hright y,type)
    # for i in range (0,nc,1):
    #     for j in range (0,nr,1):
    #         if i%2==0 and j%2==0 :
    #             k.append([i,j,1,1,9])
    #         else :
    #             k.append([i,j,1,1,3])
    
    for i in range (0,nc,1):
        for j in range (0,nr,1):
                k.append([i,j,1,1,0])
    
    # k=[[1,2],[4,5],[3,4],[6,7]]
    # k=[[6,7,4,4,0]]
    for i in k :
        block(i)

def eff(s):
    match s :
        case 1 :
            bullet()
        case 2 :
            bain()
        case 3 :
            life()
        case 4 :
            bigb()
        case 5 :
            steel()
        case 6 :
            padic()
        case 7 :
            padde()
        case 8 :
            slmo()

def proini(na,ti) :
    for i in PB :
        if i[1]['text']==na :
            PB[PB.index(i)][2]=ti
            i[0]['value']=100
            return
    r=Progressbar(f,length=150,orient=HORIZONTAL,value=100)
    r.pack(side='bottom')
    t=Label(f,text=na)
    t.pack(side='bottom')
    PB.append([r,t,ti,ti])
    pbch()

def pbch() :
    global PBB
    if len(PB) > 0 and not PBB:
        PBB = True
        x=threading.Thread(target=pbrun,daemon=True)
        x.start()
    
def pbrun() :
    global PBB
    while len(PB)>0 :
        if PA :
            for _ in range(10) :
                for i in PB :
                    i[0]['value']-=((100/(i[3]+1))/10)
                time.sleep(0.1)
            for i in PB :
                i[2]-=1
            
                if i[2]==0 :
                    PB.remove(i)
                    i[0].destroy()
                    i[1].destroy()
        else :
            return     
    PBB=False

def prosleep(nu,co):
    td=co/10
    for _ in range(10):
        nu['value']-=td
        time.sleep(0.1)

def lifeset():
    global LIIN
    m=WD//4
    he=sc.bbox('lifelable')[3]
    p=0
    for _ in range(MAXL) :
        if p >= 4 :
            he+=50
        p%=4        
        x=sc.create_oval(p*m+10,he+40,p*m+40,he+10,fill='black')
        LIIN.append(x)
        p+=1

def life():
    global NOLI
    if NOLI<MAXL :
        sc.itemconfig(LIIN[NOLI],fill='red')
        NOLI+=1

def pri(s,ind):
    global SCORE
    sw=mode.index(s)
    if sw==0 :
        SCORE+=1
    elif sw==9 :
        x=c.coords(ind)
        p=[(x[0]+x[2])/2,(x[1]+x[3])/2]
        c.delete(ind)
        explode(p)
        SCORE+=3

    else :
        SCORE+=2
        drop(sw,ind)
    sc.itemconfig('score',text=f":  {SCORE}  :")
    w.update()
         
def drop(s,ind):
    global DRL,DRP
    co=c.coords(ind)
    w=abs(co[0]-co[2])
    h=abs(co[1]-co[3])
    r=min(w,h)/3
    w=(co[0]+co[2])/2
    h=(co[1]+co[3])/2
    k=c.create_oval(w-r,h-r,w+r,h+r,fill=colour[s],tags=mode[s])
    c.addtag_withtag('drop',k)
    DRL.append([k,s,r])
    if not DRP :
        x=threading.Thread(target=down,daemon=True) 
        x.start()
        DRP= TRUE
  
def down():
    global DRL,DRP
#                      k,s,r
    while DRL :
        if PA :
            c.move('drop',0,3)
            for i in DRL :
                co=c.coords(i[0]) 
                p=c.coords('pad')
                # print(i[2])
                if p[0]-i[2]<=co[0] and p[2]+i[2]>=co[2] :
                    if p[1]<=co[3]:
                        c.delete(i[0])
                        eff(i[1])
                        DRL.remove(i)
                        
                elif p[3]<co[3] :
                    c.delete(i[0])
                    DRL.remove(i)
        else :
            return
        time.sleep(.05)
    DRP=False

def slmo():
    global spt
    spt=20
    proini('SLOE-MO',spt)
    if OSPEED==SPEED :
        x=threading.Thread(target=slow,daemon=True)
        x.start()
        
def slow() :
    global spt,OSPEED,SPEED
    SPEED+=10
    while(spt>=0):
            if PA :
                spt-=1
                time.sleep(1)
            else :
                return
        
    SPEED=OSPEED

def target():
    global TAR
    ocol= coget('obsticle')
    missing=True
    tar=[]
    try :
        while len(ocol)>0:    
            missing=True   
            for i in tar:
                if ocol[0][0][0]==i[0] and ocol[0][0][2]==i[2] :
                    missing=False
                    if ocol[0][0][1]>i[0][1]:
                        tar.remove(i)
                        tar.append(ocol[0][0])
                        
                    
            if missing :
                
                tar.append(ocol[0])
            ocol.pop(0)
        TAR=tar      
    except IndexError:
        print('ERROR')
  
def bullet():
    global sho,SH,x
    target()
    sho=20
    proini('TURRETS',sho/2)
    if not SH :
        SH=TRUE
        x=threading.Thread(target=laun,daemon=TRUE)
        x.start()
        
def laun():
    global sho,SH
    st=True
    while sho>0 :
        if PA :
            sho-=1
            co=c.coords('pad')
            for i in range(0,3,2) :
                c.create_oval(co[i]+10,co[1]-10,co[i]-10,co[1]+10,fill='green',tags='bullet')
            if st :               
                y=threading.Thread(target=forw,daemon=True)
                y.start()
                st=False
            time.sleep(.5)
        else :
            return
    if PA :
        SH=FALSE

def shoot():
    if PA :
        co=c.coords('pad')
        for i in range(0,3,2) :
            c.create_oval(co[i]+10,co[1]-10,co[i]-10,co[1]+10,fill='green',tags='bullet')
        if not BULLMOVE :
            y=threading.Thread(target=forw,daemon=True)
            y.start()
        
def forw():
    global BULLMOVE
    BULLMOVE=TRUE
    while len(coget('bullet'))>0 :
        if PA :
            c.move('bullet',0,-20)
            idf()
            time.sleep(.1)
        else :
            return
    BULLMOVE=FALSE

def idf():
    bcol=coget('bullet')
    for i in bcol :
        # print(i[1])
        if i[0][1] < 0 :
            c.delete(i[1])
        if i[0][1] < 400 :
            for j in TAR :
                buhit(i,j)
                
def buhit(bco,oco):
    global POS
    try :
        if oco[0][2]>bco[0][0]>(oco[0][0]-20) :    
            if oco[0][3]>bco[0][1] :
                kill(oco[1])
                c.delete(bco[1])        
                
    except IndexError :
        print(bco,' ',oco,' buhit')

def coget(x):
    a=c.find_withtag(x)
    co=[]
    for i in a :
        co.append([c.coords(i),i])
    return co

def hitbox():
    global t,WS,HS
    t=0
    bl=[]
    # i=False
    b=c.coords('ball')
    # print(b,' AS B')
    bc=[(b[0]+b[2])//2,(b[1]+b[3])//2]
    for i in POS :
        # print('a ',i)
        if hit(i,b):
            print('hit')
            bl.append(i)
    
    if bl :
        bl=decide(bl,bc,b)
            
        for i in bl :
            print('ok')
            kill(i)
        if bl :                
            match t :
                case 0 :
                    pass
                case 1 :
                    HS=-HS
                case 2 :
                    WS=-WS
                case 3 :
                    HS=-HS
                    WS=-WS

def decide(db,mp,bp):
    x=[]
    
    # col=[]
    # for co in db :
    #     col.append(c.coords(i))
    
    for i in db :
        co=c.coords(i)
        cbc=(co[0]+co[2])//2
        f=True
        for j in [co[1],co[3]] :
            if distance(mp,[cbc,j]) < 50 :
                    x.append(i)
                    f=False
                    break
        if f :
            for j in [co[0],co[2]] :
                for k in [co[1],co[3]] :
                    if distance(mp,[j,k]) < 50 :
                        x.append(i)
                        break
                else :
                    continue
                break
    # if x :    
        # print(x)    
    return x
    
def distance(a,b) :
    i=int((((a[0]-b[0])**2)+((a[1]-b[1])**2))**(1/2))
    if i <100 :
        print(i,'  ',a,'  ',b,"   in dist")
    return(i)

def kill(ind) :
    d=c.gettags(ind)
    try :
        
        # print(d,i)
        pri(d[0],ind)
        c.delete(ind)
        if ind in POS :
            POS.remove(ind)
        if BULLMOVE :
            target()
    except :
        print(d,'  well'  ,ind)
        exit()

def hit(x,b):
    global t
    o=c.coords(x)
    # v=[abs(bc[0]-o[0]),abs(bc[0]-o[2]),abs(bc[1]-o[1]),abs(bc[1]-o[3])]

    # for i in v :
    #     if i<(SS/2) :
            # print('in')
    try :
        if o[2]>b[0]>(o[0]-SP) :
            if o[1]<=b[3]<(o[1]+HS) or o[3]-HS>b[1]>o[3] :
                # print(v)
                # print(b,'  ',bc)
                if DEF :
                    # print('height')
                    if t==0 :
                        t=1
                    if t==2 :
                        t=3
                    if int(NS)<SP :
                        
                        rock()
                return True

        if o[3]>b[1]>(o[1]-SP) :
            if o[0]<=b[2]<(o[0]+WS) or o[2]-WS>b[0]>=o[2] :
                if DEF:
                    # print('side')
                    if t==0 :
                        t=2
                    if t==1 :
                        t=3
                    if int(NS)<SP :
                        rock()
                    return True

        if o[2]>b[0]>(o[0]-SP) :
            if o[1]<=b[3]<(o[3]) or o[3]>b[1]>o[1] :
                if DEF:
                    t=3
                    # print('rutli tut tue')
                    if int(NS)<SP :
                        rock()
                return True
        return False
    except IndexError :
        print(b,' ',o,'  CAUGHT')
            # break

def play(dir):

    x=c.coords('pad')
    if PA:
        x=c.coords('pad')
        if dir:
            if x[2]<WI :
                c.move('pad',PS,0)
        else:
            if x[0]>0 :
                c.move('pad',-PS,0)
    elif AIM :
        global DEG
        a=70/((WI/2)/PS)
        x=c.coords('pad')
        if dir:
            if x[2]<WI :
                c.move('pad',PS,0)
                c.move('ball',PS,0)
                DEG+=a
        else:
            if x[0]>0 :
                c.move('pad',-PS,0)
                c.move('ball',-PS,0)
                DEG-=a

def mopl(e):  # move the pad
    try :
        if MW and PA:
        # if MW :
            pa=c.coords('pad') 
            p=(pa[0]+pa[2])//2
            m=e.x
            if PW<=m<WI- PW :            
                c.move('pad',m-p,0)
    except IndexError :
        pass

def onof():
    global MW
    MW=not MW

def bigb():
    global bb,NS
    bb=3
    proini('BIG BALL',bb)
    if SP == NS :
        bb=10
        x=threading.Thread(target=inc,daemon=TRUE)
        x.start()

def inc():
    global bb,SP
    if SP==int(NS) :
        SP*=3
        cop=c.coords('ball') 
        x=(cop[0]+cop[2])//2
        y=(cop[1]+cop[3])//2
        c.delete('ball')
        c.create_oval(x-(SP//2),y-(SP//2),x+(SP//2),y+(SP//2),fill=BALLCOL,tags='ball')
        if not DEF :
            c.itemconfig('ball',fill='grey')
            
    while(bb>=0):
        if PA :
            bb-=1
            time.sleep(1)
        else :
            return
        
    SP=int(NS)
    cop=c.coords('ball') 
    x=(cop[0]+cop[2])//2
    y=(cop[1]+cop[3])//2
    c.delete('ball')
    c.create_oval(x-(SP//2),y-(SP//2),x+(SP//2),y+(SP//2),fill=BALLCOL,tags='ball')
    if not DEF :
        c.itemconfig('ball',fill='grey')
    
def steel():
    global nd,DEF
    nd=10
    proini('HARDNED BALL',nd)
    if DEF :
        x=threading.Thread(target=harden,daemon=TRUE)
        x.start()

def harden():
    global DEF,nd
    DEF= False
    c.itemconfig('ball',fill='grey')
    while(nd>=0):
        if PA :
            nd-=1
            time.sleep(1)
        else :
            return
    DEF= TRUE
    c.itemconfig('ball',fill=BALLCOL)

def padc():
    global HS
    b=c.coords('ball')
    pa=c.coords('pad')
    
    
    if b[1]>H :
        gameover()
    q=pa[1]+HS
    if q>b[3]>=pa[1]:
        
        m=(b[0]+b[2])//2
        
        if pa[0]<m<pa[2] :
            if HS>0:
                
                bounce()
    elif b[3]>=pa[1]:
        m=(b[0]+b[2])//2
        if m<pa[0]<b[2] or b[0]<pa[2]<m :
            corner()
            
def corner() :
    global WS,HS
    b=c.coords('ball')
    pa=c.coords('pad')
    m=(b[0]+b[2])//2
    pac=(pa[0]+b[2])//2
    if pac>m :
        k=(pa[0]-m)/(SP/70)
    else:
        k=140-(m-pa[2])/(SP/70)
    WS,HS = cord(k,MD)
            
def bounce() :
    global WS,HS
    b=c.coords('ball')
    pa=c.coords('pad')
    k=(((b[0]+b[2])//2)-pa[0])/(PW/70)
    WS,HS = cord(k,MD)

def gameover() :
    global NOLI
    
    for i in PB :
        i[0].destroy()
        i[1].destroy()
    if NOLI > 0 :
        c.delete('ball','pad')
        gloreset()
        setpos()
        NOLI-=1
        sc.itemconfig(LIIN[NOLI],fill='black')
        
    else :
        time.sleep(0.5)
        c.delete(ALL)
        w.update()
        time.sleep(1)
        c.create_text(c.winfo_width()/2, c.winfo_height()/2,font=('Chiller',200,'bold'),
                  text="GAME\nOVER", fill="red", tags="gameover")

def setpos():   
    w2=(WI//2)
    # c.create_oval(800,200,840,240,fill=BALLCOL,tags='ball')
    # c.create_oval(w2-(SP//2),H-PH,w2+(SP//2),H-PH-SP,fill=BALLCOL,tags='ball')
    c.create_rectangle(w2-(SP//2),H-PH,w2+(SP//2),H-PH-SP,fill=BALLCOL,tags='ball')
    c.create_rectangle(w2-PW,H-PH,w2+PW,H,fill='red',tags='pad')

def boomball():
    if PA :
        x=c.coords('ball')
        p=[(x[0]+x[2])/2,(x[1]+x[3])/2]
        explode(p)

def clearsmole() :
    time.sleep(.2)
    c.delete('explode')
    w.update()

def explode(p) :
    r=80
    m=exploderad(p,r)

    c.create_oval(p[0]+r,p[1]+r,p[0]-r,p[1]-r,fill='springgreen',tags='explode')
    x=threading.Thread(target=clearsmole)
    x.start()
    c.lift('ball')
    w.update()
    for i in m :
        if 'obsticle' in c.gettags(i) :
            kill(i)
    w.update()

def exploderad(n,r) :
    m=[]
    p=[n[0]//SS,n[1]//SS]
    r=r//SS-1
    for i in range(-r,r+1) :
        j=r-abs(i)
        m.extend(c.find_overlapping((p[0]+i)*SS,(p[1]-j)*SS,(p[0]+i+1)*SS,(p[1]+j+1)*SS))
    return m

def create():
    setpos()
    c.create_text(c.winfo_width()//2, c.winfo_height()//1.4,
            font=('Arial Black',30,'bold'), text="PRESS SPACE TO PLAY", fill="white", tags="start")
    b=sc.create_text(sc.winfo_width()//2,0,font=('Arial Black',30,'bold','underline'), 
                     text="SCORE", fill="orange",anchor='n')
    k=sc.bbox(b)[3]
    sc.create_text(sc.winfo_width()//2,k-15,font=('Arial Black',50,'bold'),
                   text=f": {SCORE} :", fill="orange",anchor='n',tags='score')
    he=sc.bbox('score')[3]
    sc.create_text(sc.winfo_width()//2,he,font=('Arial Black',30,'bold','underline'),
                   text="life", fill="red",anchor='n',tags='lifelable')
    sc.create_line(0,655,sc.winfo_width(),655,width=7,fill='black')
    lifeset()
    blc()
    move()
    for _ in range(4):
        life()
    
def cord(d,l=10):
    k=TRUE
    if d>70:
        d=140-d
        k=FALSE
    d+=20
    s=math.tan(d*(math.pi/180))
    ss=s**2
    x=(l/(ss+1))**(1/2)
    y=x*s
    if k :
        x=-x
    return [x,-y]

def move():
    if PA :
        cheak()
        padc()
        # ballpath()
        if len(c.find_withtag('ball'))>0 :
            hitbox()
            c.move('ball',WS,HS)  
            w.after(SPEED,move)
        else :
            print('SO')
    
def ballpath() :
    co=c.coords('ball')
    # c.create_oval(co[0],co[1],co[2],co[3],fill='red')
    c.create_oval(co[0]+15,co[1]+15,co[2]-15,co[3]-15,fill='WHITE',outline='red')
    pass
    
def cheak():
    global HS,WS
    co=c.coords('ball')
    try :
        if co[0]<0 or co[2]> WI :
            WS = - WS
            if int(NS)<SP :
                rock()
        if BAR :    
            if co[1]<0 or co[3]>H-10:
                HS = - HS   
                if int(NS)<SP :
                    rock()
        elif co[1]<0 :
            
            HS = - HS   
            if int(NS)<SP :
                rock()
    except IndexError :
        # print(co)
        # time.sleep(100)
        pass

def padic():
    global PW
    PW+=5
    pa=c.coords('pad') 
    cop=(pa[0]+pa[2])//2
    c.delete('pad')
    c.create_rectangle(cop-PW,H-PH,cop+PW,H,fill='red',tags='pad')
    
def padde():
    global PW
    PW-=5
    pa=c.coords('pad') 
    cop=(pa[0]+pa[2])//2
    c.delete('pad')
    c.create_rectangle(cop-PW,H-PH,cop+PW,H,fill='red',tags='pad')

def winset():
    glodec()
    global c,sc,WD,f
    c=Canvas(w,height=H,width=WI,bg="black")
    c.pack(side='left')
    w.update()
    ww=w.winfo_width()
    sw=w.winfo_screenwidth()
    WD=sw-ww
    if WD>200 :
        WD = 200
    sc=Canvas(w,height=H,width=WD,bg="blue")
    sc.pack(side='right')
    w.update()
    wh=w.winfo_height()
    ww=w.winfo_width()
    ww=w.winfo_width()
    sh=w.winfo_screenheight()
    x=((sw-ww)//2)-7
    y=((sh-wh)//2)-40
    w.geometry(f"{ww}x{wh}+{x}+{y}")
    w.resizable(False,False)
    f=Frame(sc,bg='blue')
    f.place(x=5,y=640,anchor='sw')
    con()
    create()

def con():
    bf=Frame(sc,bg="blue")
    bf.place(x=3,y=sc.winfo_height()-40)
    fo=("Cooper Black",11,'bold')
    bre=Button(bf,text="RESTART",font=fo,command=lambda: re())
    bre.pack(side="left",padx=1)
    be=Button(bf,text="EXIT",font=fo,command= w.destroy)
    be.pack(side="left",padx=1)

def labase():
    global LAON
    pa=c.coords('pad') 
    cop=(pa[0]+pa[2])//2
    ind=c.create_polygon(pa[0]+10,H,pa[2]-10,H,cop,H//1.08,fill='white',tags='base')
    LASE.append([ind,50])
    if not LAON :
        x=threading.Thread(target=bapla,daemon=TRUE)
        x.start()
        LAON=True

def bapla():
    global LAON,LASE
    while LASE :
        if PA :
            pa=c.coords('pad')
            for i in LASE :
                cop=(pa[0]+pa[2])//2
                ba=c.bbox(i[0])
                cob=(ba[0]+ba[2])//2
                c.move(i[0],cop-cob,0)
                w.update()
                i[1]-=1
                if i[1]<0 :
                    laser(i[0])
                    LASE.remove(i)
            time.sleep(0.1)
        else :
            return
    LAON=False
    
def laser(l):
    pa=c.bbox(l)
    cop=(pa[0]+pa[2])//2
    c.create_rectangle(cop-5,H//1.1,cop+5,H,fill='white',tags='laser')
    w.update()
    time.sleep(.05)
    c.delete('laser')
    w.update()
    
    time.sleep(.1)
    c.create_rectangle(cop-5,H//2,cop+5,H,fill='white',tags='laser')
    w.update()
    time.sleep(.05)
    c.delete('laser')
    w.update()
    
    time.sleep(.1)
    c.create_rectangle(cop-5,H,cop+5,0,fill='white',tags='laser')
    w.update()
    time.sleep(.2)
    frylaser()
    w.update()
    time.sleep(.1)
    c.delete('laser')
    c.delete(l)

def frylaser():
    # ob=coget('obsticle')
    l=c.coords('laser')
    olap=c.find_overlapping(l[0],l[1],l[2],l[3])
    # print(ob)
    for i in olap :
        d=c.gettags(i)
        try :
            if d[1]=='obsticle' :
                kill(i)
                pri(d[0],i)
                c.delete(i)
                if i in POS :
                    POS.remove(i)
        except IndexError :
            pass
    if SH :
        target()        

def bain():
    global bc
    bc=10
    proini('barrier',bc)
    if not BAR : # and PA :
        x=threading.Thread(target=bari,daemon=TRUE)
        x.start()

def bari():
    global BAR,bc
    BAR= True
    c.create_rectangle(0,H,WI,H-10,fill='green',tags='barieer')
    while(bc>=0):
        if PA :
            bc-=1
            time.sleep(1)
        else :
            return
    BAR=False
    
    c.delete('barieer')

def start():
    global BEGIN,PA,MW,AIM,WS,HS
    if BEGIN :
        WS,HS = cord(DEG,MD)
        c.delete('start')
        BEGIN=False
        MW=TRUE
    PA=not PA
    if AIM :
        WS,HS = cord(DEG,MD)
        AIM=False
    if PA :
        c.delete("pause")
        weever()
    else:
        c.create_text(c.winfo_width()//2, c.winfo_height()//2.5,
                font=('Arial Black',350,'bold'), text="||", fill="white", tags="pause")
    move()

def re():
    # global BEGIN
    c.delete(ALL)
    sc.delete(ALL)
    # BEGIN=TRUE
    glodec()
    create()

def weever():
    try :
        if sho>0 :
            x=threading.Thread(target=laun,daemon=TRUE)
            x.start()
        else :
            y=threading.Thread(target=forw,daemon=True)          
            y.start()
    except NameError :
        pass
    if BAR :
        b=threading.Thread(target=bari,daemon=TRUE)
        b.start()
    if OSPEED<SPEED :
        x=threading.Thread(target=slow,daemon=True)
        x.start()
    if not DEF :
        x=threading.Thread(target=harden,daemon=TRUE)
        x.start()
    if int(NS)<SP :
        x=threading.Thread(target=inc,daemon=TRUE)
        x.start()
    if PBB :
        x=threading.Thread(target=pbrun,daemon=True)
        x.start()
    if DRP :
        x=threading.Thread(target=down,daemon=True) 
        x.start()
    if LAON :
        x=threading.Thread(target=bapla,daemon=TRUE)
        x.start()

def rock():
    x=threading.Thread(target=shake)
    x.start()
    
def shake() :
    d=0.001
    c.move(ALL,5,2)
    w.update()
    time.sleep(d)
    c.move(ALL,-5,-4)
    w.update()
    time.sleep(d)
    c.move(ALL,0,2)
    w.update()

def binder():
    winset()
    w.bind('4',lambda event: play(FALSE))
    w.bind('6',lambda event: play(TRUE))
    w.bind("<Right>",lambda event: play(TRUE))
    w.bind("<Left>",lambda event: play(FALSE))
    c.bind("<Motion>",mopl)
    w.bind("<Button-3>",lambda event: start())
    w.bind("<space>",lambda event: start())
    w.bind('z',lambda event: onof())
    w.bind('Z',lambda event: onof())

    w.bind('A',lambda event: re())
    w.bind('Q',lambda event: bain())
    w.bind('W',lambda event: bullet())
    w.bind('E',lambda event: slmo())
    w.bind('R',lambda event: labase())
    w.bind('T',lambda event: steel())
    w.bind('Y',lambda event: bigb())
    w.bind('U',lambda event: boomball())
    w.bind('I',lambda event: shoot())
    w.bind('O',lambda event: padde())
    w.bind('P',lambda event: padic())
    w.bind('L',lambda event: life())
    w.bind('M',lambda event: rock())

    w.bind('<Return>',lambda event: test())
    
def test():
    print(POS)
    
    pass

w=Tk()
if __name__ == '__main__':
    
    binder()

w.mainloop()

# Q is barrier
# W iS for shooting
# E is slow ball
# R is for LASER
# T undiverting ball
# Y big balls
# U is ball explode 2222222222222222222222222222222222222222222
# I is shoot a single bullet
# P is pad size increase
# O is pad size decrease

# life()