import subprocess
import shlex
from multiprocessing import Process
from gtts import gTTS
import speech_recognition as sr
import time

r = sr.Recognizer()

def single_player_cpp(colour):
	subprocess.call(shlex.split('./single_player.o colour'))

def double_player_cpp():
	print('Double player')
	a = subprocess.call(['./double_player.o'])
	#print(a)

def welcome():
	tts = gTTS(text='Hello! Welcome to Wizard Chess.', lang='en', slow=False)
	tts.save("welcome.mp3")
	subprocess.Popen(['mpg123', '-q', '/home/animesh/Desktop/VoiceToText/welcome.mp3']).wait()

def confirm(s):
	tts = gTTS(text = s + '. Yes or No?', lang = 'en', slow = False)
	tts.save("confirm.mp3")
	subprocess.Popen(['mpg123', '-q', '/home/animesh/Desktop/VoiceToText/confirm.mp3']).wait()
	a = 'UnknownValueError'
	while (a == 'UnknownValueError' or a == 'RequestError'):
		a = listen_input()
		if (a == 'yes'):
			return 1
		elif (a == 'no'):
			return 0
		sorry()
		speak('Yes or no?')		

def speak(s):
	tts = gTTS(text=s, lang='en', slow=False)
	tts.save("speak.mp3")
	subprocess.Popen(['mpg123', '-q', '/home/animesh/Desktop/VoiceToText/speak.mp3']).wait()


def sorry():
	tts = gTTS(text='Sorry. I could not understand that. Please speak again.', lang='en', slow=False)
	tts.save("sorry.mp3")
	subprocess.Popen(['mpg123', '-q', '/home/animesh/Desktop/VoiceToText/sorry.mp3']).wait()

BING_KEY = "f2d6f80e3cbf40ae8bdffa5e0ad25404"  # Microsoft Bing Voice Recognition API keys 32-character lowercase hexadecimal strings



def listen_input():
	with sr.Microphone() as source:
		r.adjust_for_ambient_noise(source)
		print("Say something!")
		audio = r.listen(source)
	try:
		res = r.recognize_bing(audio, key=BING_KEY, language = 'en-IN')
		return res
	except sr.UnknownValueError:
		return 'UnknownValueError'
	except sr.RequestError as e:
		return 'RequestError'
		

def detect_char(s):
	dict = {'eight':'a', 'be':'b', 'a':'a', 'b':'b', 'c' : 'c', 'd' : 'd', 'e' : 'e', 'f' : 'f', 'g': 'g', 'h': 'h', 'age' : 'h', 'it' : 'a', 'at' : 'f', 'gtu' : 'g'}
	if dict.has_key(s):
		return dict[s]
	return None 

def detect_int(s):
	dict = {'to':'2', 'for':'4', 'three': '3', 'four': '4', 'one' : '1', 'run' : '1', 'five' : '5', 'six' : '6', 'seven' : '7', 'eight' : '8', 'two' : '2', 'sex' : '6', 'gtu' : '2'}
	if dict.has_key(s):
		return dict[s]
	return None

def convert(s):
	if ((int(s) - 48) < 10):
		return int(s) - 48
	else:
		return int(s) - 96
	
def send_to_arduino(xi, yi, xf, yf, piece_cut):
	print('Reached here')
	# port = open('/dev/ttyACM5', 'w')
	# port.write(piece_cut)
	# port.write(xi)
	# print(xi)
	# port.write(yi)
	# print(yi)
	# port.write(xf)
	# print(xf)
	# port.write(yf)
	# print(yf)
	# port.close()

def single_player_arg():
	speak('Do you want to play as black or white?')
	s = listen_input()
	if (s == 'UnknownValueError' or s == 'RequestError'):
		sorry()
		print(s)
		return single_player_arg()
	return s		



if (__name__ == '__main__'):
	welcome()
	s_arg = single_player_arg()
	Process(target=single_player_cpp, args=(s_arg,)).start()
	read_file_start = "("
	#####
	comp_file_start = "("
	state = 1
	if (s_arg == 'black'):
		state = -1
	while 1:
		if (state == 1):
			speak('What do you want to move?')
			s = listen_input()
			if (s == 'UnknownValueError' or s == 'RequestError'):
				sorry()
				print(s)
				continue
			else:	
				#if (confirm('You said' + s)):
				if 1:	
					l = s.split()
					print(l)
					if (len(l) == 4):
						l0 = detect_char(l[0])
						l1 = detect_int(l[1])
						l2 = detect_char(l[2])
						l3 = detect_int(l[3])
						print (l0, l1, l2, l3)
						if ((l0 == None or l1 == None) or (l2 == None or l3 == None)):
							print(l[0], l[1], l[2], l[3], l0, l1, l2, l3)
							sorry()
							continue
						print (l0, l1, l2, l3)	
						if confirm('Moving ' + l0 + l1 + ' to ' + l2 + l3):
							file = open('coord_file','w')
							file.truncate()
							x = l0 + l1 + ' ' + l2 + l3
							file.write(x)
							file.close()
							read_file = open('write_file', 'r')
							res = read_file.readline()
							while(not(len(res) == 3 and res[0] == read_file_start)):
								read_file.close()
								read_file = open('write_file', 'r')
								res = read_file.readline()
								continue
							read_file.close()
							if (read_file_start == "("):
								read_file_start == ")"
							else:
								read_file_start == "("	
							if (res[1] == '0'):	
								speak('This is not a valid move. Please speak again.')
							elif (res[1] == '2'):
								send_to_arduino(l0, l1, l2, l3, res[2])
								####
								#state=-1;
								speak('Checkmate.')
							elif (res[1] == '3'):
								send_to_arduino(l0, l1, l2, l3, res[2])
								#####
								#state=-1;
								speak('The game ends in Draw.')		
							else:
								send_to_arduino(l0, l1, l2, l3, res[2])
								####3
								state=-1;
								time.sleep(5)
						else:
							sorry()
							continue
					elif (len(l) == 3 and l[0] == 'gtu'):
						l.insert(1, 'gtu')
						l0 = detect_char(l[0])
						l1 = detect_int(l[1])
						l2 = detect_char(l[2])
						l3 = detect_int(l[3])
						print (l0, l1, l2, l3)
						if ((l0 == None or l1 == None) or (l2 == None or l3 == None)):
							print(l[0], l[1], l[2], l[3], l0, l1, l2, l3)
							sorry()
							continue
						print (l0, l1, l2, l3)	
						if confirm('Moving' + l0 + l1 + 'to' + l2 + l3):
							file = open('coord_file','w')
							file.truncate()
							x = l0 + l1 + ' ' + l2 + l3
							file.write(x)
							file.close()
							read_file = open('write_file', 'r')
							res = read_file.readline()
							while(not(len(res) == 3 and res[0] == read_file_start)):
								continue
							read_file.close()
							if (read_file_start == "("):
								read_file_start == ")"
							else:
								read_file_start == "("	
							if (res[1] == '0'):	
								speak('This is not a valid move. Please speak again.')
							elif (res[1] == '2'):
								send_to_arduino(l0, l1, l2, l3, res[2])
								speak('Checkmate.')
							elif (res[1] == '3'):
								send_to_arduino(l0, l1, l2, l3, res[2])
								speak('The game ends in Draw.')		
							else:
								send_to_arduino(l0, l1, l2, l3, res[2])
								state = -1
								time.sleep(5)										
					else:
						sorry()
						continue
				else:
					sorry()
					continue

		elif(state==-1):
			comp_file = open('diff_file', 'r')
			res = comp_file.readline()
			while(not((len(res) == 7 or len(res)==8) and res[0] == comp_file_start)):
				comp_file.close()
				comp_file = open('diff_file', 'r')
				res = comp_file.readline()
				continue
			
			if (comp_file_start == "("):
				comp_file_start == ")"
			else:
				comp_file_start == "("
			speak('I am moving' + res[1:6])
			send_to_arduino(res[1], res[2], res[3], res[4], res[5])
			if (res[7] == '2'):
				speak('Checkmate.')
			elif (res[7] == '3'):
				speak('The game ends in Draw.')		
			state=1;
			time.sleep(15)
