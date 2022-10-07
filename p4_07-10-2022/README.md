# 4th plenary session: 07.10.2022 #

Since the deadline of the Oblig is approaching (Wednesday October 19, 23:59), in
this plenary session we focused on some small, but yet important pieces of the
whole Home Exam 1, specifically on preparing a simple hello packet with its own
hello header and sending / receiving it via RAW sockets. During the session, we
explained how to construct the packet (frame header + hello header + SDU data)
at the sender side and decode it at the receiver side.  

Check the code
[here](https://github.com/kr1stj0n/plenaries-in3230-in4230-h22/tree/main/p4_07-10-2022/ping-pong)
and feel free to adapt it for the Oblig.  

**NOTE:** These examples that we are coding in these plenaries are very simple,
and do not follow the best coding practises. Most of the time we hardcode stuff
for the sake of simplicity. Make sure to write clean and efficient code in C,
with proper functions and better comments.  

In this session we used `sendmsg()` / `recvmsg()` to send the data encapsulated
in a msg header. Another path that you can follow while working with the Home
Exams is to serialize / deserialize structure inside byte arrays. We didn't have
time to talk about this, but feel free to explore more this approach on your own.  

Here is a good start:  
<https://www.includehelp.com/c/how-to-copy-complete-structure-in-a-byte-array-character-buffer-in-c%20language.aspx>
