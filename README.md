# NixAirlines
#### A system to support the interaction between an Airline and Travel Agencies or Particulars. Main goal: Book and Manage Flights.

Written in C (unix) and all the communication are supported by namedpipes.

## How to run:
(First time) - ./gcctask.sh
- ./servC (To start Server)
- ./loggerC (To start Logger)
- ./adminC (To start Admin)
- ./terminalC (To start Terminal)

### Commands supported by the System:
#### Terminal (Travel agencies or particulars):
- login [username] [password]
- logout
- exit
- help
- changepassword [old password] [new password]
- list (Flights) 
- search [departure] [arrival]
- book [flight id] [passport id]
- cancel [flight id] [passport id]

![Screenshot](https://raw.github.com/mjgsilva/NixAirlines/master/usercredentials.png)

#### Admin: (Password: adminpassword.txt)
- login [password]
- shutdown
- addcity [cityname]
- delcity [cityname]
- seepast
- addflight [id] [departure city] [arrival city] [date]
- delflight [id]
- changedate [day]
- getdate
- info
- adduser [username] [password]
- exit
