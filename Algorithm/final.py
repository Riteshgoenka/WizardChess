from __future__ import division
import pyaudio
import wave
import pickle
import math


CHUNK = 1
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
RECORD_SECONDS = 5
WAVE_OUTPUT_FILENAME = "three2.wav"

averaging_length = 512

threshold = 0.4

class soundFile:
	def __init__(self, name, n, x,l):
		self.file_newvalues = n
		self.name = name
		self.file_x = x
		self.mfcc=l


def reverse_string(string):
	if (len(string)==8):
		return string
	else:	
		return reverse_string(string[8:])+string[0:8]

def str_to_int(string):
	if (string[0]=='0'):
		return abs(int(string[1:],2))
	else:
		return abs(int(string[1:],2) - 2 **	15)	

def average_values(l):
	res = []
	for i in range(0, int(len(l) / averaging_length - 1)):
		x = round(sum(l[i*averaging_length: (i+1)*averaging_length]) / averaging_length, 4)
		for j in range(0, averaging_length):
			res.append(x)
	return res		 	

# def standard_deviation(l1,l2,a,b,c,d):
# 	l1 = average_values(l1)
# 	l2 = average_values(l2)
# 	j
# 	t1 = 1/(b-a)
# 	t2 = 1/(d-c)
# 	i=0
# 	j=0
# 	sd=0
# 	while(i<(b-a) and j<(d-c)):
# 		if((i+1)*t1 < (j+1)*t2):
# 			sd = sd+ ((i+1)*t1-j*t2)*(abs((l1[i]-l2[j])) ** 0.125)
# 			i=i+1
# 		else:
# 			  sd = sd + ((j+1)*t2 - i*t1) * (abs((l1[i]-l2[j])) ** 0.125)
# 			j=j+1
# 	return sd**0.5	


def fft(l,p,q):
	a=0
	b=0
	c=100
	d=4000
	l3=[]
	l1=[]
	for k in range(0,int(len(l[p:q])/256)):
		l2=[]
		t=c
		while(len(l2)<12):
			if(len(l3)==21 or t == 4000):
				l2.append(sum(l3)/len(l3))
				l3=[]
			for j in range(0,256):
				a=a+l[p+j+k*256]*math.cos(2*3.14*j/256*t)
				b=b+l[p+j+k*256]*math.sin(2*3.14*j/256*t)
			l3.append((a**2+b**2)**0.5)
			a=0
			b=0
			t=t+(d-t)/256
		l1.append(normalize(l2))
	return l1

def normalize(l):
	a=0
	for i in range(0,len(l)):
		a=a+math.log10(l[i])
	a=a/len(l)
	for i in range(0,len(l)):
		l[i]=abs(math.log10(l[i])-a)
	return l

def diff(l1,l2):
	a=0
	for i in range(0,len(l1)):
		a=a+(l1[i]-l2[i])**2
	return a

def dtw(l1,l2,n,m,r):

	l3=[]
	l4=[]
	pos1=0
	pos2=0

	for j in range(0, m):
		l3.append(0)

	for i in range(0,n):
		l4.append(l3)

	while(pos1!= n-1 and pos2!= m-1):
		k=pos1
		p=pos2
		count=1
		while(k<min(p+r+int(count*n/m),n)):
				if(k==0 and p==0):
					l4[0][0]=diff(l1[k],l2[p])
				elif(p-1<0):
					l4[k][p]=l4[k-1][p]+diff(l1[k],l2[p])
				else:
					l4[k][p]=min(l4[k-1][p],l4[k][p-1],l4[k-1][p-1])+diff(l1[k],l2[p])
				k=k+1
		k=pos1
		p=p+1
		while( p<min(k+r+int(count*m/n),m)):
				if(k-1<0):
					l4[k][p]=l4[k][p-1]+diff(l1[k],l2[p])
				else:
					l4[k][p]=min(l4[k-1][p],l4[k][p-1],l4[k-1][p-1])+diff(l1[k],l2[p])
				p=p+1

		p=pos2
		if(pos1 == n-1):
			pos2=m-1
		elif(pos2 == m-1):
			pos1=n-1
		elif(pos1 == n-2 and pos2 == m-2):
			pos1=pos1+1
			pos2=pos2+1
			l4[pos1][pos2] = min(l4[pos1-1][pos2],l4[pos1][pos2-1],l4[pos1-1][pos2-1])+diff(l1[pos1],l2[pos2])
		else:	
			pos1=pos1+1
			pos2=pos2+1
			count=count+1

	return l4[n-1][m-1]/(n+m)


def separate_words(l):
	below_threshold = 0
	res = []
	start_or_end = 0   # 0 for start 1 for end
	for i in range(0,len(l)):
		if (start_or_end==0):
			if(l[i]<threshold):
				continue
			else:
				res.append(i)
				start_or_end=1
		else:
			if(l[i]<threshold and below_threshold>40000):
				res.append(i-40000)
				start_or_end=0
				below_threshold=0
			else :
				if(l[i]<threshold):
					below_threshold = below_threshold + 1
				else:
					below_threshold=0
	return res			            		

def pre_emphasise(l):
	l1=[]
	for x in range(0,len(l)-1):
		l1.append(l[x+1]-0.95*l[x])
	return l1

def dft(l):
	l1=[]
	a=0
	b=0
	for i in range(1,257):
		for j in range(0,len(l)):
			a=a+l[j]*math.cos(2*3.14*i*j/len(l))*(0.54-0.46*math.cos(2*3.14*j/len(l)))
			b=b+l[j]*math.sin(2*3.24*i*j/len(l))*(0.54-0.46*math.cos(2*3.14*j/len(l)))
		l1.append((a**2+b**2)/len(l))
	return l1

def filter(l1,l2):
	l=[]
	a=0
	for i in range(0,len(l2)-2):
		for j in range(l2[i],l2[i+1]):
			a=a+l1[j]*(j-l2[i])/(l2[i+1]-l2[i])
		for k in range(l2[i+1],l2[i+2]):
			a=a+l1[k]*(l2[i+2]-k)/(l2[i+2]-l2[i+1])
		l.append(math.log10(a))
		a=0
	return l

def dct(l,x):
	l1=[]
	a=0
	for i in range(1,x):
		for j in range (0,len(l)):
			a=a+l[j]*math.cos(i*(j-0.5)*3.14/len(l))
		l1.append(a)
		a=0
	return l1



def indexes(lopass,hipass,x):
	a=2595*math.log10(1+lopass/700)
	b=2595*math.log10(1+hipass/700)
	l=[]
	l1=[]
	k=a
	l.append(lopass)
	for i in range(1,x):
		k=a+i*(b-a)/x
		l.append(700*((10**(k/2595))-1))
	l.append(hipass)
	for i in l:
		l1.append(int(513*i/RATE))
	return l1

def mfcc(l,pointsno,interval,lopass,hipass,x,dctno):
	l1=[]
	for i in range(0,int((len(l)-pointsno)/interval)):
		j=i*interval
		l1.append(dct(filter(dft(l[j:j+pointsno]),indexes(lopass,hipass,x)),dctno))
	return l1



p = pyaudio.PyAudio()

stream = p.open(format=FORMAT,
				channels=CHANNELS,
				rate=RATE,
				input=True,
				frames_per_buffer=CHUNK)

print("* recording")

frames = []
values = []

for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
	data = stream.read(CHUNK)
	frames.append(data)
	values.append(str_to_int(reverse_string(''.join('{:08b}'.format(ord(c)) for c in data))))


print("* done recording")

values = pre_emphasise(values)
max_of_list = max(values)
newvalues = [round(i / max_of_list,2) for i in values]
# print(average_values(newvalues))


x = separate_words(newvalues)
#print len(x)

# print dft(newvalues[0:400])
# print filter(dft(newvalues[0:400]),indexes(300,8000,26))

# if(len(x)==2):
# 	print(mfcc(newvalues[x[0]:x[1]],400,150,300,4000,26,12))

# file = 'stored.pk'
# with open(file, 'rb') as fi:
# 	check_list = pickle.load(fi)

# a=fft(newvalues,x[0],x[1])
# print(len(a))
# print(a)
# b=check_list[1].fft
# print len(b)
# print b
# print(dtw(a,b,len(a),len(b),min(len(a),len(b))/2))


# print len(check_list)
# for sfile in check_list:
# 	b=sfile.fft
# 	# print(len(b))
# 	# print(b)
# 	print(dtw(a,b,len(a),len(b),min(len(a),len(b))/2))

file='test.pk'

with open(file,'rb') as fi:
	check_list = pickle.load(fi)

if(len(x)==2):
	a=mfcc(newvalues[x[0]:x[1]],400,150,300,4000,26,12)
	for sfile in check_list:
		b=sfile.mfcc
		# print(len(b))
		# print(b)
		print(dtw(a,b,len(a),len(b),min(len(a),len(b))/2))

# for sfile in check_list:
#
# 	b=sfile.fft
# 	print(dtw(a,b,len(a),len(b),int(min(len(a),len(b))/2)))

# print(len(x))

# print(len(x))

# FOR RECORDING

# if (len(x) != 2):
# 	print("Not correct")
# else:
#
#
#
# if(len(x)==2):
#
# 	three2 = soundFile('three2', newvalues, x,fft(newvalues,x[0],x[1]))
# 	file = 'stored.pk'
# 	with open(file, 'rb') as fi:
# 		dump_list = pickle.load(fi)
# 		dump_list.append(three2)
#
# 	with open(file,'wb') as fi:
# 		pickle.dump(dump_list, fi)

# file = 'test.pk'
# if(len(x)==2):

# 	three2 = soundFile('eight', newvalues, x,mfcc(newvalues[x[0]:x[1]],400,150,300,4000,26,12))
# 	with open(file, 'rb') as fi:
# 		dump_list = pickle.load(fi)
# 		dump_list.append(three2)

# 	with open(file,'wb') as fi:
# 		pickle.dump(dump_list, fi)

# file = 'test.pk'
# if(len(x)==2):

# 	three2 = soundFile('one', newvalues, x,mfcc(newvalues[x[0]:x[1]],400,150,300,4000,26,12))

# 	with open(file,'wb') as fi:
# 		pickle.dump([three2], fi)



# file='stored.pk'

# with open(file, 'rb') as fi:
# 	dump_list = pickle.load(fi)


# print (type(dump_list))


# if(len(x)==2):
# 	l=fft(newvalues, x[0], x[1])
# 	print l
# 	three2 = soundFile('eight', newvalues, x, l)
# 	file = 'stored.pk'
# 	with open(file, 'rb') as fi:
# 		dump_list = pickle.load(fi)
# 	dump_list.append(three2)	
# 	with open(file, 'wb') as fi:
# 		pickle.dump(dump_list,fi)
# 	print(len(dump_list))	


# file='stored.pk'
#
# with open(file, 'rb') as fi:
# 	dump_list = pickle.load(fi)
#
# with open(file, 'wb') as fi:
#  	pickle.dump(dump_list.pop(),fi)


# #   FOR COMPARING

# file = 'stored.pk'	
# with open(file, 'rb') as fi:
# 	check_list = pickle.load(fi)
	

# for sfile in check_list:
# 	#sn = sfile.file_newvalues
# 	print(standard_deviation(newvalues, sfile.file_newvalues, x[0], x[1], separate_words(sfile.file_newvalues)[0], separate_words(sfile.file_newvalues)[1]))	

# print(standard_deviation(sfile.file_newvalues, check_list[1].file_newvalues, separate_words(check_list[0].file_newvalues)[0], separate_words(check_list[0].file_newvalues)[1], separate_words(check_list[1].file_newvalues)[0], separate_words(check_list[1].file_newvalues)[1]))


stream.stop_stream()
stream.close()
p.terminate()

wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
wf.setnchannels(CHANNELS)
wf.setsampwidth(p.get_sample_size(FORMAT))
wf.setframerate(RATE)
wf.writeframes(b''.join(frames))
wf.close()
