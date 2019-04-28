#!/usr/bin/python
#Reffered from :- https://realpython.com/python-send-email/
#This script sends an email with the OTP generated from the control node.
import smtplib, ssl
import sys



server = smtplib.SMTP('smtp.gmail.com', 587)
server.ehlo()  # Can be omitted
server.starttls()
server.ehlo()  # Can be omittedI
my_email = "satyamehta96@gmail.com"
my_password = 'viratkohli99'

if(sys.argv[1] == "satya"):
	receiver_email = "satyamehta1996@gmail.com"
        server.login(my_email, my_password)
	message = """\
Subject: One Time Password

			
This message is sent from Remote control node. OTP:""" + sys.argv[2]
        server.sendmail(my_email, receiver_email, message)
        server.quit()
        print("Email sent to Satya")

elif(sys.argv[1] == "siddhant"):
	print (sys.argv[2])
	receiver_email = "jajoosiddhant@gmail.com"
        server.login(my_email, my_password)
	message = """\
Subject: One Time Password

			
This message is sent from Remote control node. OTP:""" + sys.argv[2]
        server.sendmail(my_email, receiver_email, message)
        server.quit()
        print("Email sent to Siddhant")
else:
    print("Email sent failed")
