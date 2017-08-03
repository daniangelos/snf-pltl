import json
from pprint import pprint
#from era import to_datetime
import datetime

timestamp = (lambda t: datetime.datetime.fromtimestamp(t))

with open('attempts.json') as json_file:
    attempts = json.load(json_file)


with open('users.json') as users_file:
    users = json.load(users_file)

def get_uid(matricula):
    user = [u["_id"] for u in users if
            u["document"]["username"] == matricula]
    return user[0]

#print(get_uid(170029875))

while(1):
    matricula = int(input())
    uid = get_uid(matricula)
    print("uid == " + str(uid))


    user_attempts = [
        (timestamp(att["document"]["when"]).strftime('%Y-%m-%d %H:%M:%S'),
        att["document"]["ip"],
        att["_id"], att["document"]["verdict"]
        ) for att in attempts 
                if att["document"]["problem"] in range(2, 29) and
                att["document"]["user"] == uid]


    user_attempts.sort(key=lambda x: x[0])
#pprint(turma)
    pprint(user_attempts)

