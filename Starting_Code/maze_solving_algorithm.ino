//#include <MemoryFree.h> 
2 int counter = 0; 
3 byte Lx = 0; // Current X coordinate 
4 byte Ly = 0; // Current Y coordinate 
5 const int Size = 7; 
6 byte centMin = 255; // The smallest index in the center square that has been found so far (the LENGTH of the current shortest path) 
7 
 
8 //TODO: continue writing methods to manipulate squareData and replace old arrays 
9 byte squareData[Size + 1][Size + 1]; //0 - Explored, 1 and 2 - Parent Direction, 3, 4, 5, 6 - Walls (N, S, E, W) 7 - virtual walls on/off 
10 byte I[Size+1][Size+1]; // Array of indicies //index array 
11 
 
12 //PARENT DIRECTION: 00 - north, 11 - south, 10 - east, 01 - west 
13 //I is the horizontal component of the matrix definition and J is the vertical component. The origin (0,0) is defined as the bottom left corner. 
14 
 
15 //vvv between is being replaced by above line 
16 boolean E[Size+1][Size+1]; // Array of explored locations 
17 
 
18 boolean Wn[Size+1][Size+1]; // Array of northern walls 
19 boolean Ws[Size+1][Size+1]; // Array of southern walls 
20 boolean Ww[Size+1][Size+1]; // Array of western walls 
21 boolean We[Size+1][Size+1]; // Array of eastern walls 
22 
 
23 boolean vWn[Size+1][Size+1]; 
24 boolean vWs[Size+1][Size+1]; 
25 boolean vWw[Size+1][Size+1]; 
26 boolean vWe[Size+1][Size+1]; 
27 
 
28 byte P[Size+1][Size+1]; 
29 //^^^ 
30 
 
31 byte whereToGo[2]; 
32 byte finalCenterSquare[2]; //holds the destination final center square 
33 byte breadCrumbs[((Size+1)^2)]; 
34 byte finishToStartDirections[((Size+1)^2)]; 
35 
 
36 
 
37 byte testSize = Size; 
38 
 
39 //const boolean testWn[8][8] = { 
40 //  {  
41 //    0,0,0,0,0,0,0,0      }  
42 //  , 
43 //  { 
44 //    1,0,0,1,1,0,1,0      }  
45 //  , 
46 //  { 
47 //    0,0,0,0,0,1,0,0      }  
48 //  , 
49 //  { 
50 //    1,0,1,0,0,0,0,0      }  
51 //  , 
52 //  { 
53 //    1,0,1,0,1,1,0,0      }  
54 //  , 
55 //  { 
56 //    1,1,0,1,0,0,0,0      }  
57 //  , 
58 //  { 
59 //    1,1,1,0,0,0,0,0      }  
60 //  , 
61 //  { 
62 //    0,0,0,1,0,1,0,0      } 
63 //}; 
64 /// 
65 //const boolean testWw[8][8] ={ 
66 //  { 
67 //    0,0,0,0,0,0,0,0        } 
68 //  , 
69 //  { 
70 //    1,0,1,0,0,1,0,0        } 
71 //  , 
72 //  { 
73 //    0,1,0,0,1,0,0,1        } 
74 //  , 
75 //  { 
76 //    0,1,0,1,1,1,0,0        } 
77 //  , 
78 //  { 
79 //    0,1,0,0,0,0,0,0      } 
80 //  , 
81 //  { 
82 //    0,0,0,1,1,0,1,1      } 
83 //  , 
84 //  { 
85 //    0,0,0,0,1,1,0,1      } 
86 //  , 
87 //  { 
88 //    0,1,0,1,0,0,0,1      } 
89 //}; 
90 // 
91 
 
92 
 
93 //revised test wall north 
94 //const boolean testWn[16][16] = { 
95 //{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
96 //{0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,1}, 
97 //{1,1,1,0,0,0,0,0,1,0,1,0,0,1,0,1}, 
98 //{1,1,1,1,1,1,0,0,0,0,0,0,1,0,1,1}, 
99 //{1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1}, 
100 //{0,1,0,0,1,0,0,0,0,0,0,0,1,1,1,1},//5 
101 //{1,0,1,0,1,1,0,0,0,1,1,1,1,1,1,1}, 
102 //{0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1}, 
103 //{1,0,1,0,1,0,1,0,0,1,1,1,1,0,0,1}, 
104 //{1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,1},//9 
105 //{1,1,1,0,0,0,1,0,1,0,1,1,1,1,1,1}, 
106 //{1,1,1,0,1,0,0,0,0,0,0,1,1,1,1,1}, 
107 //{1,1,1,1,0,1,0,1,0,0,0,1,1,1,1,1},//12 
108 //{1,1,1,0,0,0,0,0,1,1,0,1,0,0,1,1}, 
109 //{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1}, 
110 //{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}}; 
111 // 
112 // 
113 // 
114 //const boolean testWw[16][16] = { 
115 //  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, 
116 //{1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,0}, 
117 //{0,0,1,1,1,1,1,0,0,0,0,1,0,0,1,1}, 
118 //{0,0,0,0,0,1,1,1,1,1,1,0,1,0,0,0}, 
119 //{0,0,0,1,0,0,0,1,1,1,0,1,0,1,1,0},//3 
120 //{0,1,1,1,1,0,1,1,1,1,1,1,0,0,0,0}, 
121 //{0,1,0,1,0,0,1,1,1,1,0,0,0,0,0,0},//5 
122 //{0,0,1,0,1,0,0,1,1,0,0,0,0,0,0,0}, 
123 //{0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0}, 
124 //{0,0,1,0,1,0,1,1,1,0,0,0,0,1,1,0},//8 
125 //{0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0}, 
126 //{0,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0}, 
127 //{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},//11 
128 //{0,0,0,0,1,0,1,1,0,0,1,1,0,0,0,0}, 
129 //{0,0,0,1,1,1,1,1,1,0,1,0,1,1,1,0},//13 
130 //{0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0}, 
131 //}; 
132 
 
133 
 
134 
 
135 ////decker maze 
136 //const boolean testWn[16][16] = { 
137 //{0,0,0,0,0,1,0,1,0,0,0,1,0,0,1,1}, 
138 //{0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1}, 
139 //{0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1}, 
140 //{1,0,0,1,1,1,0,0,0,1,1,1,0,1,0,1}, 
141 //{0,0,1,0,1,1,0,1,1,1,1,1,1,1,1,1}, 
142 //{1,1,1,0,1,0,0,1,1,1,1,1,1,1,1,1}, 
143 //{1,1,1,0,1,1,0,1,0,0,0,1,1,0,1,1},//6 
144 //{1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,1}, 
145 //{1,1,1,1,1,1,1,0,1,0,0,0,0,1,1,1}, 
146 //{1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,1}, 
147 //{1,1,1,1,1,0,0,0,1,1,1,0,1,0,0,1}, 
148 //{1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1}, 
149 //{0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,1}, 
150 //{1,1,0,1,1,1,0,0,0,0,0,0,1,0,0,1},//13 
151 //{1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1}, 
152 //{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1} 
153 //}; 
154 // 
155 ////decker maze 
156 //const boolean testWw[16][16] = { 
157 //  
158 //  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, 
159 //  {1,1,1,0,1,0,0,1,0,1,1,1,1,1,0,0}, 
160 //  {0,1,1,1,1,0,0,0,1,1,0,0,1,1,1,0}, 
161 //  {0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0}, 
162 //  {0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}, 
163 //  {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0}, 
164 //  {0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0}, 
165 //  {0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,0}, 
166 //  {0,1,0,1,0,0,0,0,0,1,1,1,1,0,0,0}, 
167 //  {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0}, 
168 //  {0,0,0,0,0,0,1,1,0,1,0,0,0,1,1,0}, 
169 //  {0,1,0,0,1,1,1,1,1,0,0,0,0,1,0,1}, 
170 //  {0,0,0,1,0,0,0,0,0,1,1,1,0,1,1,0}, 
171 //  {0,1,1,0,0,0,1,1,0,1,0,1,1,0,1,1}, 
172 //  {0,0,1,0,0,1,1,1,1,1,1,1,0,1,1,0}, 
173 //  {0,1,1,0,0,1,1,1,1,1,0,1,0,0,0,0} 
174 //}; 
175      
176 //boolean testWs[Size+1][Size+1]; 
177 //boolean testWe[Size+1][Size+1]; 
178 
 
179 
 
180 
 
181 void setup() { 
182   //Start the Serial Monitor and print out debugging info 
183   Serial.begin(115200); 
184 //  Serial.print("freeMemory()="); 
185 //  Serial.println(freeMemory()); 
186 //  Serial.println("Initialization Started"); 
187   //initTestWalls(); 
188    
189   //Initialize the various matricies we use to map out the maze 
190   //Note that for some of the arrays we are initializing we are initializing it to one  
191   destroyVirtualWalls(); //initialize virtual walls to zero 
192   initializeWalls(); 
193   initializeParents(); 
194   for (int i = 0; i < ((Size+1)^2); i++) { 
195     breadCrumbs[i] = 4; 
196     finishToStartDirections[i] = 4; 
197   } 
198   whereToGo[0] = Size+1; 
199   whereToGo[1] = Size+1; 
200   //Note initializing these to Size+1 is important to the GoneToFar function  TODO: Fix this 
201   finalCenterSquare[0] =Size+1; 
202   finalCenterSquare[1] =Size+1; 
203    
204    I[0][0] = 1;  //Set the index of the starting square to one 
205   Serial.println("Initialization Finished"); 
206 } 
207 
 
208 void loop() { 
209   while(!isCenter()) { 
210     updateWalls(); 
211     updateIndicies(); 
212     correctLinearity(); 
213     updateCentMin; 
214     manhattanSweep(); 
215     deadEndSweep(); 
216      parentSweep(); 
217 //Output current robot data 
218 //    Serial.print(Lx); 
219 //    Serial.print(","); 
220 //    Serial.print(Ly); 
221 //    Serial.print(" Index:"); 
222 //    Serial.print(I[Lx][Ly]); 
223 //    Serial.println(); 
224 //    Serial.print("Current Parent is = "); 
225 //    Serial.println(P[Lx][Ly]); 
226 //    Serial.println(); 
227 //    outputMaze(); 
228     if(goneTooFar(Lx,Ly)) { 
229       setAllVirtualWalls(Lx,Ly); 
230       moveUnit(P[Lx][Ly]); 
231     } else { 
232     moveUnit(moveDir()); 
233     } 
234   } 
235       //run all the methods one more time to map the center 
236     updateWalls(); 
237     updateIndicies(); 
238     correctLinearity(); 
239     manhattanSweep(); 
240     deadEndSweep(); 
241      parentSweep(); 
242     outputMaze(); 
243   
244    
245   //Needs to run these methods again to update center square 
246   visitCenter(); 
247 
 
248 
 
249   Serial.print("Hansel and Gretel"); 
250   hanselAndGretel(goFromCenterToStart()); 
251    
252   if(foundShortest()) { 
253     Serial.print("SPEED RACER"); 
254     while(true) { 
255     hanselAndGretel(goFromStartToSquare(finalCenterSquare[0],finalCenterSquare[1])); 
256     hanselAndGretel(goFromCenterToStart()); 
257     Serial.print("Update Motor Speed"); 
258     delay(50); 
259     } 
260   } 
261    
262   whereDoWeGoNow(); 
263   hanselAndGretel(goFromStartToSquare(whereToGo[0],whereToGo[1])); 
264 } 
265 
 
266 //void initTestWalls() { 
267 //  for(int i = 0; i <= testSize; i++) { 
268 //    for(int j = 0; j <= testSize; j++) { 
269 //      if(testWn[i][j]) { 
270 //        setTestNorthWall(i,j); 
271 //      } 
272 //      if(testWw[i][j]) { 
273 //        setTestWestWall(i,j);  
274 //      } 
275 //      //      if(j==0) { 
276 //      //        testWs[i][j] = 1; 
277 //      //      } 
278 //      //      if(i==testSize) { 
279 //      //        testWe[i][j] = 1;  
280 //      //      } 
281 //    } 
282 //  } 
283 //} 
284 
 
285 
 
286 
 
287 //void setTestNorthWall(int lx, int ly) { 
288 //  if(ly < Size) { 
289 //    testWs[lx][ly+1] = true; 
290 //  } 
291 //} 
292 // 
293 //void setTestWestWall(int lx, int ly) { 
294 //  if(lx > 0) { 
295 //    testWe[lx-1][ly] = true; 
296 //  } 
297 //} 
298 
 
299 /* initializeSquareData 
300 *  sets all eight bits to 0 for each square in the maze 
301 *  and adds the outer walls (sets to 1) 
302 */ 
303 void intializeSquareData() 
304 { 
305   for (int i = 0; i < Size; i++) 
306   { 
307     for (int j = 0; j < Size; j++) 
308     { 
309 
 
310       bitClear(squareData[i][j] ,0); //sets bit 0 to 0 
311       bitClear(squareData[i][j] ,1); //sets bit 1 to 0 
312       bitClear(squareData[i][j] ,2); //sets bit 2 to 0 
313       bitClear(squareData[i][j] ,3); //sets bit 3 to 0 
314       bitClear(squareData[i][j] ,4); //sets bit 4 to 0 
315       bitClear(squareData[i][j] ,5); //sets bit 5 to 0 
316       bitClear(squareData[i][j] ,6); //sets bit 6 to 0 
317       bitClear(squareData[i][j] ,7); //sets bit 7 to 0 
318        
319     }  
320      
321   }   
322    
323   //Set South walls 
324   for (int q = 0; q <= Size; q++) 
325   { 
326     bitSet(squareData[0][q] ,4); 
327   } 
328    
329   //Set North walls 
330   for (int q = 0; q <= Size; q++) 
331   { 
332     bitSet(squareData[Size][q] ,3); 
333   } 
334    
335   //Set West walls 
336   for (int q = 0; q <= Size; q++) 
337   { 
338     bitSet(squareData[q][0] ,6); 
339   } 
340    
341   //Set East walls 
342   for (int q = 0; q <= Size; q++) 
343   { 
344     bitSet(squareData[q][Size] ,5); 
345   } 
346 } 
347 
 
348 
 
349 /* setExploredBit 
350 *  given an x and y location 
351 *  sets the bit for explored to 1 (true) 
352 */ 
353 void setExploredBit(int lx, int ly) 
354 { 
355    bitSet(squareData[lx][ly], 0); 
356 } 
357 
 
358 /* 
359 *  Overload setExploredBit - pass bool as well to  
360 *  set explored to 0 (false) 
361 */ 
362 void setExploredBit(int lx, int ly, boolean flag) 
363 { 
364    bitClear(squareData[lx][ly], 0); 
365 } 
366 
 
367 /* setParentBit 
368 *  given an x and y location, as well as a single 0 - 3 int, 
369 *  sets the parent of the block at (x,y)  
370 *  by changing the values of bits 1 and 2 
371 *  0 - North 
372 *  1 - South 
373 *  2 - East 
374 *  3 - West 
375 */ 
376 void setParentBit(int lx, int ly, int q) 
377 { 
378   //north (00) 
379   if (q == 0) 
380   { 
381     bitClear(squareData[lx][ly], 1);  
382     bitClear(squareData[lx][ly], 2); 
383   } 
384    
385   //south (11) 
386   else if (q == 1) 
387   { 
388     bitSet(squareData[lx][ly], 1); 
389     bitSet(squareData[lx][ly], 2); 
390   } 
391    
392   //east (10) 
393   else if (q == 2) 
394   { 
395     bitSet(squareData[lx][ly], 1); 
396     bitClear(squareData[lx][ly], 2); 
397   } 
398    
399   //west (01) 
400   else if (q == 3) 
401   { 
402     bitClear(squareData[lx][ly], 1); 
403     bitSet(squareData[lx][ly], 2); 
404   } 
405    
406   else  
407   { 
408     Serial.println("Invalid parent parameter");  
409   } 
410    
411 } 
412 
 
413 /* setWallBitNorth 
414 *  given coordinates of square, sets its north wall (and the south wall of the one above it) 
415 *  to true (1)  
416 *  by changing bit 3 
417 */ 
418 void setWallBitNorth(int lx, int ly) 
419 { 
420    //add wall 
421     bitSet(squareData[lx][ly], 3); 
422     if (ly < Size) 
423     { 
424       bitSet(squareData[lx][ly + 1], 4); //sets south of neighboring square 
425     } 
426 
 
427 } 
428    
429 /* 
430 *  Overload- add boolean flag to set 
431 *  to 0 (false) instead 
432 */ 
433 void setWallBitNorth(int lx, int ly, boolean flag) 
434 { 
435    bitClear(squareData[lx][ly], 3); 
436    if (ly < Size) 
437    { 
438      bitClear(squareData[lx][ly + 1], 4); //clears south of neighboring square 
439    } 
440 } 
441 
 
442 /* setWallBitEast 
443 *  given coordinates of square, sets its east wall (and the west wall of the one to its right) 
444 *  to true (1)  
445 *  by changing bit 5 
446 */ 
447 void setWallBitEast(int lx, int ly) 
448 { 
449     bitSet(squareData[lx][ly], 5); 
450     if (lx < Size) 
451     { 
452       bitSet(squareData[lx + 1][ly], 6); //sets west of neighboring square 
453     } 
454    
455 } 
456 
 
457 /* 
458 *  Overload - add boolean flag to  
459 *  set to 0 (false) instead 
460 */ 
461 void setWallBitEast(int lx, int ly, boolean flag) 
462 { 
463     bitClear(squareData[lx][ly], 5); 
464     if (lx < Size) 
465     { 
466       bitClear(squareData[lx + 1][ly], 6); //clears west of neighboring square 
467     } 
468 
 
469     
470 } 
471 
 
472 /* setWallBitsSouth 
473 *  given coordinates of square, sets its south wall (and the north wall of the one under it) 
474 *  to true (1) 
475 *  by changing bit 4 
476 */ 
477 void setWallBitSouth(int lx, int ly) 
478 { 
479    bitSet(squareData[lx][ly], 4);  
480    if (ly != 0) 
481    { 
482      bitSet(squareData[lx][ly - 1], 3); //sets north of neighboring square 
483    } 
484 
 
485 } 
486 
 
487 /* 
488 *  Overload - pass a boolean flag to  
489 *  set value to false (0) instead 
490 */ 
491 void setWallBitSouth(int lx, int ly, boolean flag) 
492 {  
493    bitClear(squareData[lx][ly], 4); 
494    if (ly != 0) 
495    { 
496      bitClear(squareData[lx][ly - 1], 3); //clears north of neighboring square 
497    } 
498 } 
499 
 
500 /* setWallBitWest 
501 *  given coordinates of a square, sets its west wall (and the east wall of the one to its left) 
502 *  to true (1)  
503 *  by changing bit 6 
504 */ 
505 void setWallBitWest(int lx, int ly) 
506 { 
507     bitSet(squareData[lx][ly], 6); 
508     if (lx != 0) 
509     { 
510       bitSet(squareData[lx - 1][ly], 5);  //sets east of neighboring square 
511     } 
512 } 
513 
 
514 /* 
515 *  Overload - pass a boolean flag to 
516 *  set value to flase (0) instead 
517 */ 
518 void setWallBitWest(int lx, int ly, boolean flag) 
519 { 
520     bitClear(squareData[lx][ly], 6);  
521     if (lx != 0) 
522     { 
523       bitClear(squareData[lx - 1][ly], 5); //clears east of neighboring square 
524     } 
525 } 
526 
 
527 /*  
528 *  setVirtualWallsBit 
529 *  sets a square to be boxed by virtual walls  
530 *  1 - boxed, 0 - unboxed 
531 *  all four walls done at once 
532 */ 
533 void setVirtualWallsBit(int lx, int ly) 
534 { 
535    bitSet(squareData[lx][ly], 7);    
536 } 
537 
 
538 
 
539 /* 
540 *  Overload - pass in a boolean flag to remove all four walls instead 
541 *  (value 0) 
542 */ 
543 void setVirtualWallsBit(int lx, int ly, boolean flag) 
544 { 
545    bitClear(squareData[lx][ly], 7); 
546 } 
547 
 
548 /* 
549 *  getExploredBit 
550 *  returns true if the given square is explored 
551 *  and false if it is not yet explored 
552 */ 
553 boolean getExploredBit(int lx, int ly) 
554 { 
555   if (bitRead(squareData[lx][ly], 0) == 1) 
556   { 
557     return true; //is explored 
558   } 
559    
560   else if (bitRead(squareData[lx][ly], 0) == 0) 
561   { 
562     return false; //is not explored 
563   } 
564 } 
565 
 
566 /* 
567 *  getParentBit 
568 *  returns an int to indicate the direction of the parent 
569 *  of the given square 
570 *  0 = north 
571 *  1 = south 
572 *  2 = east 
573 *  3 = west 
574 */ 
575 int getParentBit(int lx, int ly) 
576 { 
577   if ((bitRead(squareData[lx][ly], 1) == 0) && (bitRead(squareData[lx][ly], 2) == 0)) 
578   { 
579     return 0; //north 
580   } 
581    
582   if ((bitRead(squareData[lx][ly], 1) == 0) && (bitRead(squareData[lx][ly], 2) == 1)) 
583   { 
584     return 1; //south 
585   } 
586    
587   if ((bitRead(squareData[lx][ly], 1) == 1) && (bitRead(squareData[lx][ly], 2) == 0)) 
588   { 
589     return 2; //east 
590   } 
591    
592   if ((bitRead(squareData[lx][ly], 1) == 1) && (bitRead(squareData[lx][ly], 2) == 1)) 
593   { 
594     return 3; //west 
595   } 
596 
 
597 } 
598 
 
599 boolean getWallBitNorth(int lx, int ly) 
600 { 
601   if (bitRead(squareData[lx][ly], 3) == 1) 
602   { 
603      return true; 
604   } 
605 
 
606   else if (bitRead(squareData[lx][ly], 3) == 0) 
607   { 
608     return false; 
609   } 
610 } 
611 
 
612 boolean getWallBitSouth(int lx, int ly) 
613 { 
614    if (bitRead(squareData[lx][ly], 4) == 1) 
615    { 
616       return true; 
617    } 
618   
619    else if (bitRead(squareData[lx][ly], 4) == 0) 
620    {  
621       return false; 
622    } 
623 } 
624 
 
625 boolean getWallBitEast(int lx, int ly) 
626 { 
627    if (bitRead(squareData[lx][ly], 5) == 1) 
628    { 
629       return true; 
630    } 
631   
632    else if (bitRead(squareData[lx][ly], 5) == 0) 
633    {  
634       return false; 
635    } 
636 
 
637 
 
638 } 
639 
 
640 boolean getWallBitWest(int lx, int ly) 
641 { 
642    if (bitRead(squareData[lx][ly], 6) == 1) 
643    { 
644       return true; 
645    } 
646   
647    else if (bitRead(squareData[lx][ly], 6) == 0) 
648    {  
649       return false; 
650    } 
651 
 
652 
 
653 } 
654 
 
655 boolean getVirtualWallsBit(int lx, int ly) 
656 { 
657    if (bitRead(squareData[lx][ly], 7) == 1) 
658    { 
659       return true; 
660    } 
661   
662    else if (bitRead(squareData[lx][ly], 7) == 0) 
663    {  
664       return false; 
665    } 
666 
 
667 
 
668 } 
669 
 
670 
 
671 /* initializeParents 
672 *  Initializes the parents array to all fours (Since four is an invalid number for a parent if the four is ever used an runtime error will occur) 
673 *  This method also initializes whereToGo to Size+1, again an invalid number 
674 *  TODO: Figure out a better way to initialize these values instead of using an invalid number 
675 */ 
676 void initializeParents() { 
677   Serial.println("Initialize Parents"); 
678   for(int i =0; i <= Size; i++) { 
679     for(int j = 0; j <= Size; j++) { 
680       P[i][j] = 4; 
681     } 
682   } 
683   whereToGo[0] = Size+1; 
684   whereToGo[1] = Size+1; 
685 } 
686 
 
687 
 
688 // initialize all parent bits to 00 (north) as default. Default is irrelevant 
689 void initializeParentBit() 
690 { 
691   Serial.println("Initialize Parents"); 
692   for (int i = 0; i <= Size; i++) 
693   { 
694     for (int j = 0; i <= Size; j++) 
695     { 
696       bitClear(squareData[i][j], 1); 
697       bitClear(squareData[i][j], 2); 
698     }  
699   } 
700    
701   whereToGo[0] = Size + 1; 
702   whereToGo[1] = Size + 1; 
703 } 
704 
 
705 
 
706 /* updateCentMin 
707 *  Checks the indices of all the center squares and makes sure centMin is set to the lowest values 
708 *  TODO: Make it so that this method is not dependant on finalCenterSquare being initialized to higher than Size 
709 *  can be left as is for now 
710 */ 
711 void updateCentMin() { 
712   if(finalCenterSquare[0]<Size) {      //TODO: Make it so that this mtethod is not dependant on finalCenterSquare being initialized to higher than Size 
713    centMin = I[finalCenterSquare[0]][finalCenterSquare[1]];  
714   } 
715 } 
716 
 
717 
 
718 
 
719 
 
720 
 
721 /* initializeWalls 
722 *  Initializes all the wall physical wall matrices to 0, except for the outer walls which are set to 1.(Since we know them to be there) 
723 *  Also Initializes all the indices to 0 and the Explored Square Matrix to 0 
724 *  now replaced by code in initializeSquareData() 
725 */ 
726 void initializeWalls() { 
727   Serial.println("Initialize Walls"); 
728   for (int i = 0; i <= Size; i++) { 
729     for (int j = 0; j <= Size; j++) { 
730       Ww[i][j] = 0; 
731       We[i][j] = 0; 
732       Ws[i][j] = 0; 
733       Wn[i][j] = 0; 
734       I[i][j] = 0; 
735       E[i][j] = 0; 
736       if (i == 0) { 
737         Ww[i][j] = 1; 
738       } 
739       if (i == Size) { 
740         We[i][j] = 1; 
741       } 
742       if (j == 0) { 
743         Ws[i][j] = 1; 
744       } 
745       if (j == Size) { 
746         Wn[i][j] = 1; 
747       } 
748     } 
749   } 
750 } 
751 
 
752 /*  destroyVirtualWalls 
753  *  Sets all virtual walls to zero in the entire matrix 
754  *  Also, used to initialize the virtual walls 
755  *  this is now carried out in initializeSquareData() 
756  */ 
757 void destroyVirtualWalls() { 
758   for (int i = 0; i <= Size; i++) { 
759     for (int j = 0; j <= Size; j++) { 
760       vWw[i][j] = 0; 
761       vWe[i][j] = 0; 
762       vWs[i][j] = 0; 
763       vWn[i][j] = 0; 
764     } 
765   } 
766 } 
767 
 
768 /* isCenter 
769 *  Returns true if the robot is physically in one of the four center squares 
770 *  doesn't require updating 
771 */ 
772 boolean isCenter() { 
773   if((Lx == (Size/2) && Ly == (Size/2)) || (Lx == (Size/2) && Ly == ((Size/2)+1)) || ((Lx == (Size/2)+1) && Ly == (Size/2)) || ((Lx == (Size/2)+1) && Ly == ((Size/2)+1))) { 
774     return true; 
775   } 
776   else { 
777     return false; 
778   } 
779 } 
780 
 
781 
 
782 
 
783 /* isCenter 
784 *  Returns true if the passed in square is one of the four center squares 
785 */  
786 boolean isCenter(int lx, int ly) { 
787   if((lx == (Size/2) && ly == (Size/2)) || (lx == (Size/2) && ly == ((Size/2)+1)) || ((lx == (Size/2)+1) && ly == (Size/2)) || ((lx == (Size/2)+1) && ly == ((Size/2)+1))) { 
788     return true; 
789   } 
790   else { 
791     return false; 
792   } 
793 } 
794 
 
795 /* visitCenter 
796 *  This function is supposed to be run when the robot first reaches a center square 
797 *  Once we reach the center we know which of the 4 center squares is our "destination square." By knowing the "destination square" we can improve our sweep functions by using that square 
798 *  in particular instead of considering all four center squares. 
799 *  This method updates center 4 squares and creates this "destination square" 
800 *  WARNING: -This method sets the physical walls around all the center squares to true. It relies on the parent of the "destination square" to remove the improperly set physical wall 
801 *           -This method also assumes you have run all the sweeps and linearity checks 
802 * 
803 void visitCenter(){ 
804   centMin = I[Lx][Ly];		//Update centMin with the index of your current square 
805    
806   //Figure out the matrix indices for all the center squares 
807   int low = (Size/2); 
808   int high = (Size/2)+1; 
809  
810   //Even though we haven't technically explored all the center squares we know everything about them so we can say we've explored them all 
811   E[low][low] = 1; 
812   E[low][high] = 1; 
813   E[high][low] = 1; 
814   E[high][high] = 1; 
815  
816   //Sets all the physical walls of all four of the center squares to true 
817   setNorthWall(low,low); 
818   setSouthWall(low,low); 
819   setEastWall(low,low); 
820   setWestWall(low,low); 
821  
822   setNorthWall(low,high); 
823   setSouthWall(low,high); 
824   setEastWall(low,high); 
825   setWestWall(low,high);  
826  
827   setNorthWall(high,high); 
828   setSouthWall(high,high); 
829   setEastWall(high,high); 
830   setWestWall(high,high);  
831  
832  
833   setNorthWall(high,low); 
834   setSouthWall(high,low); 
835   setEastWall(high,low); 
836   setWestWall(high,low);  
837  
838   //Sets the "destination square" to our current location 
839   finalCenterSquare[0]=Lx; 
840   finalCenterSquare[1]=Ly; 
841    
842   //Delete the physical wall that is blocking the entrance to the center that was improperly set above 
843   if(P[Lx][Ly] == 0) { 
844     Wn[Lx][Ly] = 0; 
845     Ws[Lx][Ly+1] = 0; 
846   }  
847   else if(P[Lx][Ly] == 1) { 
848     Ws[Lx][Ly] = 0; 
849     Wn[Lx][Ly-1] = 0; 
850   }  
851   else if(P[Lx][Ly] == 2) { 
852     We[Lx][Ly] = 0; 
853     Ww[Lx+1][Ly] = 0; 
854   }  
855   else if (P[Lx][Ly] == 3) { 
856     Ww[Lx][Ly] = 0; 
857     We[Lx-1][Ly] = 0; 
858   } 
859 } 
860 */ 
861 
 
862 /* updated visitCenter 
863 *  This function is supposed to be run when the robot first reaches a center square 
864 *  Once we reach the center we know which of the 4 center squares is our "destination square." By knowing the "destination square" we can improve our sweep functions by using that square 
865 *  in particular instead of considering all four center squares. 
866 *  This method updates center 4 squares and creates this "destination square" 
867 *  WARNING: -This method sets the physical walls around all the center squares to true. It relies on the parent of the "destination square" to remove the improperly set physical wall 
868 *           -This method also assumes you have run all the sweeps and linearity checks 
869 */ 
870 void visitCenter() 
871 { 
872   centMin = I[Lx][Ly];		//Update centMin with the index of your current square 
873    
874   //Figure out the matrix indices for all the center squares 
875   int low = (Size/2); 
876   int high = (Size/2)+1; 
877 
 
878   //Even though we haven't technically explored all the center squares we know everything about them so we can say we've explored them all 
879   bitSet(squareData[low][low], 0); 
880   bitSet(squareData[low][high], 0); 
881   bitSet(squareData[high][low], 0); 
882   bitSet(squareData[high][high], 0); 
883 
 
884    
885   //Sets all the physical walls of all four of the center squares to true 
886   setWallBitNorth(low,low); 
887   setWallBitSouth(low,low); 
888   setWallBitEast(low,low); 
889   setWallBitWest(low,low); 
890 
 
891   setWallBitNorth(low,high); 
892   setWallBitSouth(low,high); 
893   setWallBitEast(low,high); 
894   setWallBitWest(low,high); 
895    
896   setWallBitNorth(high,low); 
897   setWallBitSouth(high,low); 
898   setWallBitEast(high,low); 
899   setWallBitWest(high,low); 
900 
 
901   setWallBitNorth(high, high); 
902   setWallBitSouth(high, high); 
903   setWallBitEast(high, high); 
904   setWallBitWest(high, high); 
905 
 
906   //Sets the "destination square" to our current location 
907   finalCenterSquare[0]=Lx; 
908   finalCenterSquare[1]=Ly; 
909    
910   //Delete the physical wall that is blocking the entrance to the center that was improperly set above 
911   if(getParentBit(Lx, Ly) == 0) { 
912     setWallBitNorth(Lx, Ly, false); //boolean flag indicates clearing a wall 
913     setWallBitSouth(Lx, Ly + 1, false); 
914   }  
915   else if(getParentBit(Lx, Ly) == 1) { 
916     setWallBitSouth(Lx, Ly, false); 
917     setWallBitNorth(Lx, Ly - 1, false); 
918   }  
919   else if(getParentBit(Lx, Ly) == 2) { 
920     setWallBitEast(Lx, Ly, false); 
921     setWallBitWest(Lx + 1, Ly, false); 
922   }  
923   else if (getParentBit(Lx, Ly) == 3) { 
924     setWallBitWest(Lx, Ly, false); 
925     setWallBitEast(Lx - 1, Ly, false); 
926   } 
927 } 
928 
 
929 
 
930 
 
931 // Reads sensor data and determines if there is a wall to the north 
932 boolean isNorthWall() { 
933 //TODO: Insert sensor code here 
934 } 
935 // Reads sensor data and determines if there is a wall to the south 
936 boolean isSouthWall() { 
937 //TODO: Insert sensor code here 
938 } 
939 // Reads sensor data and determines if there is a wall to the west 
940 boolean isWestWall() { 
941 //TODO: Insert sensor code here 
942 } 
943 // Reads sensor data and determines if there is a wall to the east 
944 boolean isEastWall() { 
945 //TODO: Insert sensor code here 
946 } 
947 
 
948 
 
949 /* moveUnit 
950 *  Pass in the desired direction you want the robot to move(N = 0, S = 1, E = 2, W = 3) 
951 *  TODO: Motor code needs to be added 
952 */ 
953 void moveUnit(int x){ 
954   if (x == 0) { 
955     //motor code 
956     Ly = Ly+1; 
957   } 
958   else if (x == 1) { 
959     Ly = Ly -1; 
960   } 
961   else if (x == 2) { 
962     Lx = Lx + 1; 
963   } 
964   else if (x == 3) { 
965     Lx = Lx -1; 
966   } 
967 } 
968 
 
969 /* numberOfWalls 
970 *  Returns the number of walls detectetd by the sensors in the square the robot is in 
971 *  Never used, instead the two parameter function is used in dead end sweep 
972 * 
973 int numberOfWalls(){ 
974   int numberOfWalls = 0; //Keeps count of number of walls 
975   if(isNorthWall()) { 
976     numberOfWalls++;  
977   } 
978   if(isSouthWall()) { 
979     numberOfWalls++;  
980   } 
981   if(isEastWall()) { 
982     numberOfWalls++;  
983   } 
984   if(isWestWall()) { 
985     numberOfWalls++;  
986   } 
987 //  Serial.print("\n#Walls: "); 
988 //  Serial.println(numberOfWalls); 
989   return numberOfWalls; 
990 } 
991 */ 
992 
 
993 /* numberOfWalls 
994 *  Never used, instead the two parameter function is used in dead end sweep 
995 * 
996 int numberOfWalls(int x){ 
997   int numberOfWalls = 0; //keeps count of number of walls 
998   if ((x == 2) || (x == 3)){ 
999     if(isNorthWall()) { 
1000       numberOfWalls++;  
1001     } 
1002     if(isSouthWall()) { 
1003       numberOfWalls++;  
1004     } 
1005   } 
1006   if ((x == 0) || (x == 1)){ 
1007     if(isEastWall()) { 
1008       numberOfWalls++;  
1009     } 
1010     if(isWestWall()) { 
1011       numberOfWalls++;  
1012     } 
1013   } 
1014   Serial.print("\n#Walls: "); 
1015   Serial.println(numberOfWalls); 
1016   return numberOfWalls; 
1017 } 
1018 */ 
1019 
 
1020 /* set****Wall 
1021  * The set wall methods should always be used when mapping walls as the set the walls of the square you are in as well  
1022  * as the complementary walls of the adjacent squares 
1023  * For example setting the west wall of one square will set the east wall of the adjacent square to the west 
1024  * obsolete with new set methods 
1025 void setNorthWall(int lx, int ly) { 
1026   Serial.print("Setting North Wall; "); 
1027   Wn[lx][ly] = true; 
1028   if(ly < Size) { 
1029     Ws[lx][ly+1] = true; 
1030   } 
1031 } 
1032  
1033 void setSouthWall(int lx, int ly) { 
1034   Serial.print("Setting South Wall; "); 
1035   Ws[lx][ly] = true; 
1036   if(ly > 0) { 
1037     Wn[lx][ly-1] = true; 
1038   } 
1039 } 
1040  
1041 void setWestWall(int lx, int ly) { 
1042   Serial.print("Setting West Wall; "); 
1043   Ww[lx][ly] = true; 
1044   if(lx > 0) { 
1045     We[lx-1][ly] = true; 
1046   } 
1047 } 
1048  
1049 void setEastWall(int lx, int ly) { 
1050   Serial.print("Setting East Wall; "); 
1051   We[lx][ly] = true; 
1052   if(lx < Size) { 
1053     Ww[lx+1][ly] = true; 
1054   } 
1055 } 
1056 */ 
1057 
 
1058 
 
1059 /* setVirtual*****Wall 
1060 *  Performs the same task as the normal set wall method but with virtual walls 
1061 *  obsolete due to new set method 
1062 */ 
1063 void setVirtualNorthWall(int lx, int ly) { 
1064   vWn[lx][ly] = true; 
1065   if(ly < Size) { 
1066     vWs[lx][ly+1] = true; 
1067   } 
1068 } 
1069 
 
1070 void setVirtualSouthWall(int lx, int ly) { 
1071   vWs[lx][ly] = true; 
1072   if(ly > 0) { 
1073     vWn[lx][ly-1] = true; 
1074   } 
1075 } 
1076 
 
1077 void setVirtualWestWall(int lx, int ly) { 
1078   vWw[lx][ly] = true; 
1079   if(lx > 0) { 
1080     vWe[lx-1][ly] = true; 
1081   } 
1082 } 
1083 
 
1084 void setVirtualEastWall(int lx, int ly) { 
1085   vWe[lx][ly] = true; 
1086   if(lx < Size) { 
1087     vWw[lx+1][ly] = true; 
1088   } 
1089 } 
1090 
 
1091 
 
1092 /* getWall**** 
1093 *  Returns true if either the real or virtual wall is set in the specified position 
1094 *  obsolete because of new get methods 
1095 */ 
1096 boolean getWallNorth(int lx, int ly) { 
		if(bitRead(squareData[lx][ly],3) || bitRead(squareData[lx][ly],7)) {
1098     return true;  
1099   } 
1100   return false;
1101 } 
1102 
 
1103 boolean getWallSouth(int lx, int ly) { 
1104   if(bitRead(squareData[lx][ly],4) || bitRead(squareData[lx][ly],7)) { 
1105     return true;  
1106   } 
1107   return false; 
1108 } 
1109 
 
1110 boolean getWallWest(int lx, int ly) { 
1111   if(bitRead(squareData[lx][ly],6) || bitRead(squareData[lx][ly],7)) { 
1112     return true;  
1113   } 
1114   return false; 
1115 } 
1116 
 
1117 boolean getWallEast(int lx, int ly) { 
1118   if(bitRead(squareData[lx][ly],5) || bitRead(squareData[lx][ly],7)) { 
1119     return true;  
1120   } 
1121   return false; 
1122 } 
1123 
 
1124 /* updateWalls 
1125 *  Reads in sensor values and sets the walls of the square the robot is in accordingly 
1126 * 
1127 void updateWalls() { 
1128   if(isNorthWall() == true) { 
1129     setNorthWall(Lx, Ly); 
1130   } 
1131   if(isSouthWall() == true) { 
1132     setSouthWall(Lx, Ly); 
1133   } 
1134   if(isWestWall() == true) { 
1135     setWestWall(Lx, Ly); 
1136   } 
1137   if(isEastWall() == true) { 
1138     setEastWall(Lx, Ly); 
1139   } 
1140 } 
1141 */ 
1142 
 
1143 
 
1144 // updateWalls 
1145 // new, updated version of updateWalls 
1146 void updateWalls() 
1147 { 
1148   if(isNorthWall()) 
1149  { 
1150    setWallBitNorth(Lx, Ly); 
1151  }  
1152   
1153  if(isSouthWall()) 
1154  { 
1155    setWallBitSouth(Lx, Ly); 
1156  }  
1157   
1158  if(isEastWall()) 
1159  { 
1160    setWallBitEast(Lx, Ly); 
1161  }  
1162   
1163  if(isWestWall()) 
1164  { 
1165    setWallBitWest(Lx, Ly); 
1166  }  
1167    
1168    
1169 } 
1170 
 
1171 /*VIRTUAL TEST METHOD 
1172 * Reads in values from the test wall matrices and then sets the actual wall matrices 
1173 */ 
1174 //void updateWalls() { 
1175 //  if(testWn[Lx][Ly]) { 
1176 //    setNorthWall(Lx,Ly); 
1177 //  }  
1178 //  if(testWs[Lx][Ly]) { 
1179 //    setSouthWall(Lx,Ly); 
1180 //  }  
1181 //  if(testWw[Lx][Ly]) { 
1182 //    setWestWall(Lx,Ly); 
1183 //  }  
1184 //  if(testWe[Lx][Ly]) { 
1185 //    setEastWall(Lx,Ly); 
1186 //  }  
1187 //} 
1188 
 
1189 
 
1190 /* updateIndices 
1191 *  Updates adjacent, unexplored, squares with no walls inbetween with appropriate index numbers and parent numbers 
1192 *  Aslo updates the Explored matrix for the square you are presently in 
1193 *  TODO: Rename this function to something more indicative of what it does 
1194 *  TODO: continue updating starting here 
1195 */ 
1196 void updateIndicies() { 
1197   E[Lx][Ly] = true; 
1198   if((!Wn[Lx][Ly]) && (Ly < Size)) { 
1199     if(I[Lx][Ly+1] == 0) { 
1200       I[Lx][Ly+1] = I[Lx][Ly]+1; 
1201       P[Lx][Ly+1] = 1; 
1202     } 
1203   } 
1204   if((!Ws[Lx][Ly]) && (Ly > 0)) { 
1205     if(I[Lx][Ly-1] == 0) { 
1206       I[Lx][Ly-1] = I[Lx][Ly]+1; 
1207       P[Lx][Ly-1] = 0; 
1208     } 
1209   } 
1210   if((!Ww[Lx][Ly]) && (Lx > 0)) { 
1211     if(I[Lx-1][Ly] == 0) { 
1212       I[Lx-1][Ly] = I[Lx][Ly]+1; 
1213       P[Lx-1][Ly] = 2; 
1214     } 
1215   } 
1216   if((!We[Lx][Ly]) && (Lx < Size)) { 
1217     if(I[Lx+1][Ly] == 0) { 
1218       I[Lx+1][Ly] = I[Lx][Ly]+1; 
1219       P[Lx+1][Ly] = 3; 
1220     } 
1221   } 
1222 } 
1223 
 
1224 /* checkLinearity 
1225 *  Checks to see if there are discrepancies between the indices of adjacent squares without walls in between them  
1226 *  (i.e A 13 adjacent to a 9 with no wall in between) 
1227 *  If a discrepancy is found correctLinearity is called to fix it 
1228 *  TODO: I beleive checkLinearity might never be called and correctLinearity is called instead 
1229 */ 
1230 void checkLinearity() { 
1231   if (!Wn[Lx][Ly]) { 
1232     if(abs(I[Lx][Ly]-I[Lx][Ly+1]) > 1) { 
1233       correctLinearity(); 
1234       return; 
1235     } 
1236   } 
1237   if (!Ws[Lx][Ly]) { 
1238     if(abs(I[Lx][Ly]-I[Lx][Ly-1]) > 1) { 
1239       correctLinearity(); 
1240       return; 
1241     } 
1242   } 
1243   if (!Ww[Lx][Ly]) { 
1244     if(abs(I[Lx][Ly]-I[Lx-1][Ly]) > 1) { 
1245       correctLinearity(); 
1246       return; 
1247     } 
1248   } 
1249   if (!We[Lx][Ly]) { 
1250     if(abs(I[Lx][Ly]-I[Lx+1][Ly]) > 1) { 
1251       correctLinearity(); 
1252       return; 
1253     } 
1254   } 
1255 } 
1256 
 
1257 /* correctLinearity 
1258 *  Checks to see if there are discrpeancies between the indices of adjacent squares without walls in between them  
1259 *  (i.e A 13 adjacent to a 9 with no wall in between) 
1260 *  If a discrepancy is found it will be corrected and linearity will be restored 
1261 */ 
1262 void correctLinearity() { 
1263   //Serial.println("Correcting Linearity"); 
1264   //We must get rid of all the virtual walls because after the linearity correction paths that were previously ruled out might be viable 
1265   destroyVirtualWalls();     
1266   boolean flag = true;      //Set a boolean flag which will cause correct linearity to keep looping until the entire maze is linear 
1267   while(flag) { 
1268     flag = false; 
1269     for (int i = 0; i <= Size; i ++) {      //Loop through the maze 
1270       for (int j = 0; j <= Size; j++) { 
1271         if (E[i][j]) {                      //We are only concerned with the linearity of explored squares as they are the only squares where we know where the walls are 
1272           // North Wall 
1273           if (!Wn[i][j]) { 
1274             if ((I[i][j] - I[i][j+1]) > 1) { 
1275               I[i][j] = I[i][j+1]+1;        //If there is a wall to the North with lower  
1276               flag = true; 
1277             } 
1278             else if (((I[i][j] - I[i][j+1]) < -1)) { 
1279               I[i][j+1] = I[i][j]+1; 
1280               flag = true; 
1281             } 
1282           } 
1283           // South Wall 
1284           if (!Ws[i][j]) { 
1285             if ((I[i][j] - I[i][j-1]) > 1) { 
1286               I[i][j] = I[i][j-1]+1; 
1287               flag = true; 
1288             } 
1289             else if (((I[i][j] - I[i][j-1]) < -1)) { 
1290               I[i][j-1] = I[i][j]+1; 
1291               flag = true; 
1292             } 
1293           } 
1294           // West Wall 
1295           if (!Ww[i][j]) { 
1296             if ((I[i][j] - I[i-1][j]) > 1) { 
1297               I[i][j] = I[i-1][j]+1; 
1298               flag = true; 
1299             } 
1300             else if (((I[i][j] - I[i-1][j]) < -1)) { 
1301               I[i-1][j] = I[i][j]+1; 
1302               flag = true; 
1303             } 
1304           } 
1305           // East Wall 
1306           if (!We[i][j]) { 
1307             if ((I[i][j] - I[i+1][j]) > 1) { 
1308               I[i][j] = I[i+1][j]+1; 
1309               flag = true; 
1310             } 
1311             else if (((I[i][j] - I[i+1][j]) < -1)) { 
1312               I[i+1][j] = I[i][j]+1; 
1313               flag = true; 
1314             } 
1315           } 
1316         } 
1317       } 
1318     } 
1319   } 
1320 } 
1321 
 
1322 
 
1323 /* Dead end sweep will look for all squares that have 3 walls, virtual or real, and then set all 4 virtual walls of that square true 
1324  *  In addition it will look for all squares that only have lower index squares adjacent to it and set all virtual walls true in that square(think a corner that has lower index squares next to it 
1325  *  Dead end sweep will continue to loop through until it has found no more dead ends or squares with only lower adjacent index values 
1326  *  Finally Dead end sweep will never set the virtual walls of the staring square(index 1), the square the robot is currently in(in order to prevent boxing the robot in, and finally any of the center squares 
1327  *  Not walling the first and center squares ensures that you will never fill in your shortest path and will only wall off invalid squares(i.e squares you don't want to visit 
1328  *  NOTE: You should usually run dead end sweep after the Manhattan Sweep as the Manhattan sweep should create some dead ends that need filling 
1329  */ 
1330 void deadEndSweep() { 
1331   //First let's run throgh and find any squares that have three walls set 
1332   boolean flag = true; 
1333   while(flag) { 
1334     flag = false;  
1335     for (int i = 0; i <= Size; i ++) { 
1336       for (int j = 0; j <= Size; j++) { 
1337         if ((I[i][j] != 1) && !((Lx == i)&&(Ly == j)) && (!isCenter(i,j))) { 
1338           //          if(numberOfWalls(i,j) == 3) { 
1339           //            Serial.print(" Three Walls in: "); 
1340           //            Serial.print(i); 
1341           //            Serial.print(" ,"); 
1342           //            Serial.println(j); 
1343           //            setAllVirtualWalls(i,j); 
1344           //            flag = true; 
1345           //          } 
1346           if(onlyLowerIndicies(i,j)) { 
1347 //            Serial.print(" Only lower indexes in: "); 
1348 //            Serial.print(i); 
1349 //            Serial.print(" ,"); 
1350 //            Serial.println(j); 
1351             setAllVirtualWalls(i,j); 
1352             flag = true; 
1353           } 
1354         } 
1355       } 
1356     } 
1357   } 
1358 } 
1359 
 
1360 // Small functions that returns true if a square is surrounded by only lower indexed squares. 
1361 // Used as a short cut in the dead end sweep. 
1362 // TODO: update this method to use new data 
1363 boolean onlyLowerIndicies(int lx, int ly) { 
1364   int flag = 0; 
1365   if(!getWallNorth(lx, ly)) { 
1366     if(I[lx][ly+1] == (I[lx][ly]-1)) { 
1367       flag++; 
1368     } 
1369   } 
1370   if(!getWallSouth(lx, ly)) { 
1371     if(I[lx][ly-1] == (I[lx][ly]-1)) { 
1372       flag++; 
1373     } 
1374   } 
1375   if(!getWallEast(lx, ly)) { 
1376     if(I[lx+1][ly] == (I[lx][ly]-1)) { 
1377       flag++; 
1378     } 
1379   } 
1380   if(!getWallWest(lx, ly)) { 
1381     if(I[lx-1][ly] == (I[lx][ly]-1)) { 
1382       flag++; 
1383     } 
1384   } 
1385   if (((4-flag) == numberOfWalls(lx,ly)) && (flag >= 1)) { 
1386     return true; 
1387   } 
1388   else { 
1389     return false; 
1390   } 
1391 } 
1392 
 
1393 // Sweeps through the known maze and sets all virtual walls to true on any square whose Manhattan Distance plus the index is greater than the current index of the center squares. 
1394 // 
1395 void manhattanSweep() { 
1396   for(int i = 0; i <= Size; i++) { 
1397     for(int j = 0; j <= Size; j++) { 
1398       if(I[i][j] != 0) { 
1399         if(getExploredBit(i,j) != 0 { 
1400         if(goneTooFar(i,j)) { 
1401           if(!isCenter(i,j)) { 
1402 //            Serial.print("Gone Too Far in: "); 
1403 //            Serial.print(i); 
1404 //            Serial.print(", "); 
1405 //            Serial.println(j); 
1406             setAllVirtualWalls(i,j); 
1407           } 
1408         } 
1409         } 
1410       } 
1411     } 
1412   } 
1413 } 
1414 
 
1415   void parentSweep() { 
1416   Serial.print("Correcting Parents: "); 
1417   for(int lx = 0; lx <= Size; lx++) { 
1418     for(int ly = 0; ly <= Size; ly++) { 
1419   if (lx>Size) { 
1420   } else if (ly>Size) { 
1421      
1422   }else if(isCenter(lx,ly)){ 
1423    //do nothing 
1424   } else { 
1425   if(!getWallNorth(lx,ly)) { 
1426     if(ly < Size){ 
1427       if((I[lx][ly]-1) == I[lx][ly+1]) { 
1428         P[lx][ly] = 0; 
1429       } 
1430     } 
1431   } 
1432   if(!getWallSouth(lx,ly)) { 
1433     if(ly > 0){ 
1434       if((I[lx][ly]-1) == I[lx][ly-1]) { 
1435         P[lx][ly] = 1; 
1436       } 
1437     } 
1438   } 
1439   if(!getWallEast(lx,ly)) { 
1440     if(lx < Size){ 
1441       if((I[lx][ly]-1) == I[lx+1][ly]) { 
1442         P[lx][ly] = 2; 
1443       } 
1444     } 
1445   } 
1446   if(!getWallWest(lx,ly)) { 
1447     if(lx > 0){ 
1448       if((I[lx][ly]-1) == I[lx-1][ly]) { 
1449         P[lx][ly] = 3; 
1450       } 
1451     } 
1452   } 
1453     } 
1454     } 
1455   } 
1456 } 
1457 
 
1458 // Small function that returns the number of walls(virtual or real) the specified square has 
1459 // Used in the deadEndSweep function 
1460 int numberOfWalls(int lx, int ly) { 
1461   int numberOfWalls = 0; //keeps count of number of walls 
1462   if(getWallNorth(lx,ly)) { 
1463     numberOfWalls++;  
1464   } 
1465   if(getWallSouth(lx,ly)) { 
1466     numberOfWalls++;  
1467   } 
1468   if(getWallWest(lx,ly)) { 
1469     numberOfWalls++;  
1470   } 
1471   if(getWallEast(lx,ly)) { 
1472     numberOfWalls++;  
1473   } 
1474   return numberOfWalls; 
1475 } 
1476 
 
1477 //Sets all the virtual walls of the specified square to true 
1478 //Used in the dead end sweep 
1479 void setAllVirtualWalls(int lx, int ly) { 
1480   //  Serial.print("Setting Virtual walls"); 
1481   if(!isCenter(lx,ly)) { 
1482     setVirtualNorthWall(lx,ly); 
1483     setVirtualSouthWall(lx,ly); 
1484     setVirtualWestWall(lx,ly); 
1485     setVirtualEastWall(lx,ly); 
1486   } 
1487 } 
1488 
 
1489 
 
1490 // Decides whether a certain square is a viable next step in relation to moving to the center 
1491 boolean goneTooFar(int lx, int ly) { 
1492   int movesTo = 255; 
1493   int high = (Size/2)+1; 
1494   int low = (Size/2); 
1495    
1496   if(finalCenterSquare[0]!=(Size+1) && finalCenterSquare[1]!=(Size+1)) { 
1497     movesTo = (abs(lx-finalCenterSquare[0])+abs(ly-finalCenterSquare[1])); 
1498   } else { 
1499   if (abs(lx-low) + abs(ly-low) < movesTo) { 
1500     movesTo = (abs(lx-low)+abs(ly-low)); 
1501   } 
1502   if (abs(lx-high) + abs(ly-low) < movesTo) { 
1503     movesTo = (abs(lx-high)+abs(ly-low)); 
1504   } 
1505   if (abs(lx-low) + abs(ly-high) < movesTo) { 
1506     movesTo = (abs(lx-low)+abs(ly-high)); 
1507   } 
1508   if (abs(lx-high) + abs(ly-high) < movesTo) { 
1509     movesTo = (abs(lx-high)+abs(ly-high)); 
1510   } 
1511   } 
1512   if ((movesTo + I[lx][ly]) > centMin) { 
1513     return true; 
1514   } 
1515   else { 
1516     return false; 
1517   } 
1518 } 
1519 
 
1520 
 
1521 // Returns THE direction to move 
1522 // 0 - NORTH 
1523 // 1 - SOUTH 
1524 // 2 - EAST 
1525 // 3 - WEST 
1526 int moveDir() { 
1527    
1528   double smallestMD = 257; // Smallest Manhattan distance variable. Initialized high to check for oddities later 
1529   int dir = 4; // Direction to move! Arbitrarily initialized at to an INVALID value 
1530   // CHECKS NORTH 
1531   if (!getWallNorth(Lx,Ly)) { // If there's not a wall 
1532     if (!getExploredBit(Lx,Ly+1)) { // And it's not visited 
1533       double movesTo = movesToCenter(Lx,Ly+1); 
1534       if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
1535         smallestMD = movesTo; // It updates the smallest manhattan distance 
1536         dir = 0; // And sets the relevant direction 
1537       } 
1538     } 
1539   } 
1540   // CHECKS SOUTH 
1541   if (!getWallSouth(Lx,Ly)) { // If there's not a wall 
1542     if (!getExploredBit(Lx,Ly-1)) { // And it's not visited 
1543       double movesTo = movesToCenter(Lx,Ly-1); 
1544       if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
1545         smallestMD = movesTo; // It updates the smallest manhattan distance 
1546         dir = 1; // And sets the relevant direction 
1547       } 
1548     } 
1549   } 
1550   // CHECKS EAST 
1551   if (!getWallEast(Lx,Ly)) { // If there's not a wall 
1552     if (!getExploredBit(Lx+1,Ly)) { // And it's not visited 
1553     double movesTo = movesToCenter(Lx+1,Ly); 
1554       if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
1555         smallestMD = movesTo; // It updates the smallest manhattan distance 
1556         dir = 2; // And sets the relevant direction 
1557       } 
1558     } 
1559   } 
1560   // CHECKS WEST 
1561   if (!getWallWest(Lx,Ly)) { // If there's not a wall 
1562     if (!getExploredBit(Lx-1,Ly)) { // And it's not visited 
1563       double movesTo = movesToCenter(Lx-1,Ly); 
1564       if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
1565         smallestMD = movesTo; // It updates the smallest manhattan distance 
1566         dir = 3; // And sets the relevant direction 
1567       } 
1568     } 
1569   } 
1570   // CHECKS FOR ODDITIES (There were no unexplored options) 
1571   if (smallestMD == 257) { 
1572     //NORTH 
1573     if (!getWallNorth(Lx,Ly)) { // If there's not a wall 
1574       if ((I[Lx][Ly]+1) == I[Lx][Ly+1] ) { 
1575         double movesTo = movesToCenter(Lx,Ly+1); 
1576         if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
1577           smallestMD = movesTo; // It updates the smallest manhattan distance 
1578           dir = 0; // And sets the relevant direction 
1579         } 
1580       } 
1581     } 
1582     //SOUTH 
1583     if (!getWallSouth(Lx,Ly)) { // If there's not a wall 
1584       if ((I[Lx][Ly]+1) == I[Lx][Ly-1] ) { 
1585         double movesTo = movesToCenter(Lx,Ly-1); 
1586         if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
1587           smallestMD = movesTo; // It updates the smallest manhattan distance 
1588           dir = 1; // And sets the relevant direction 
1589         } 
1590       } 
1591     } 
1592     //EAST 
1593     if (!getWallEast(Lx,Ly)) { // If there's not a wall 
1594       if ((I[Lx][Ly]+1) == I[Lx+1][Ly] ) { 
1595         double movesTo = movesToCenter(Lx+1,Ly); 
1596         if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
1597           smallestMD = movesTo; // It updates the smallest manhattan distance 
1598           dir = 2; // And sets the relevant direction 
1599         } 
1600       } 
1601     } 
1602     //WEST 
1603     if (!getWallWest(Lx,Ly)) { // If there's not a wall 
1604       if ((I[Lx][Ly]+1) == I[Lx-1][Ly] ) { 
1605         double movesTo = movesToCenter(Lx-1,Ly); 
1606         if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
1607           smallestMD = movesTo; // It updates the smallest manhattan distance 
1608           dir = 3; // And sets the relevant direction 
1609         } 
1610       } 
1611     } 
1612   } 
1613   if (smallestMD == 257) { 
1614     if (!getWallNorth(Lx,Ly)) { 
1615       dir = 0; 
1616     } 
1617     if (!getWallSouth(Lx,Ly)) { 
1618       dir = 1; 
1619     } 
1620     if (!getWallEast(Lx,Ly)) { 
1621       dir = 2; 
1622     } 
1623     if (!getWallWest(Lx,Ly)) { 
1624       dir = 3; 
1625     } 
1626   } 
1627    
1628   if (dir == 4) { 
1629     while(true) { 
1630       Serial.print("INVALID MOVE"); 
1631       delay(1000); 
1632     } 
1633   }  else{ 
1634 
 
1635   //Serial.print(" Direction returned"); 
1636   //Serial.println(dir); 
1637   return dir; // returns the chosen one 
1638     } 
1639 } 
1640 
 
1641 double movesToCenter(int lx,int ly) { 
1642   double movesTo; 
1643   double sizeInDouble = Size; 
1644   double sizeDivTwo = sizeInDouble/2; 
1645    
1646   movesTo = abs(sizeDivTwo-lx)+abs(sizeDivTwo-ly); 
1647 
 
1648 return movesTo; 
1649 } 
1650 
 
1651 
 
1652 int readBackwards(int i) { 
1653   if(i == 0) { 
1654     return 1; 
1655   } 
1656   if(i == 1) { 
1657     return 0; 
1658   } 
1659   if(i == 2) { 
1660     return 3; 
1661   } 
1662   if(i == 3) { 
1663     return 2; 
1664   } 
1665 } 
1666 
 
1667 
 
1668 void whereDoWeGoNow() { 
1669   int lowest = 257; 
1670   int lowestLx = Size+1; 
1671   int lowestLy = Size+1; 
1672   for(int i = 0; i <= Size; i++) { 
1673     for(int j = 0; j <=Size; j++) { 
1674       if(!E[i][j]) { 
1675         if(!isCenter(i,j)) { 
1676         if(!goneTooFar(i,j)){ 
1677         if(numberOfWalls(i,j)!=4) { 
1678           if(I[i][j] != 0) { 
1679             if(I[i][j] < lowest) { 
1680               lowest = I[i][j]; 
1681               lowestLx = i; 
1682               lowestLy = j; 
1683             } 
1684           } 
1685         } 
1686           } 
1687         } 
1688       } 
1689     } 
1690   } 
1691   Serial.println(); 
1692   Serial.println("############"); 
1693   Serial.println("Where do we go now: "); 
1694   Serial.print(lowestLx); 
1695   Serial.print(", "); 
1696   Serial.println(lowestLy); 
1697   Serial.println("############"); 
1698   if(lowestLx==Size+1) { 
1699     whereToGo[0] = finalCenterSquare[0]; 
1700     whereToGo[1] = finalCenterSquare[1]; 
1701  Serial.println("No Valid whereToGo Square"); 
1702   } else{ 
1703      
1704      whereToGo[0] = lowestLx; 
1705   whereToGo[1] = lowestLy; 
1706   } 
1707 } 
1708 
 
1709 int goFromStartToSquare(int lx, int ly) { 
1710   int i = lx; 
1711   int j = ly; 
1712   int c = 0; 
1713 
 
1714   while(I[i][j] != 1) { 
1715     breadCrumbs[c] = readBackwards(getParentBit(i,j)); 
1716     c++; 
1717     if(getParentBit(i,j) == 0) { 
1718       j=j+1; 
1719     } 
1720     else if(getParentBit(i,j) == 1) { 
1721       j=j-1; 
1722     } 
1723     else if(getParentBit(i,j) == 2) { 
1724       i= i + 1; 
1725     } 
1726     else if(getParentBit(i,j) == 3) { 
1727       i= i - 1; 
1728     } 
1729   } 
1730   //flip breadcrumbs 
1731   int d = c; 
1732   int temp = 0; 
1733   for (int i = 0; i < d; i++) { 
1734     temp = breadCrumbs[i]; 
1735     breadCrumbs[i] = breadCrumbs[d-1]; 
1736     breadCrumbs[d-1] = temp; 
1737     d--; 
1738   } 
1739 //  Serial.println("Breadcrumbs: "); 
1740 //  for(int i = 0; i < c; i++){ 
1741 //    Serial.print(breadCrumbs[i]); 
1742 //    Serial.print(", "); 
1743 //  } 
1744   return c; 
1745 } 
1746 
 
1747 int goFromCenterToStart() { 
1748   //  Serial.println("From Center to Star"); 
1749   //  Serial.println("##############"); 
1750   int i = Lx; 
1751   int j = Ly; 
1752   int c =0; 
1753   while(I[i][j] != 1) { 
1754     //    Serial.print("Location: "); 
1755     //    Serial.print(i); 
1756     //    Serial.print(", "); 
1757     //    Serial.println(j); 
1758     //    delay(1000); 
1759     breadCrumbs[c] = getParentBit(i,j); 
1760     c++; 
1761     if(getParentBit(i,j) == 0) { 
1762       j=j+1; 
1763     } 
1764     else if(getParentBit(i,j) == 1) { 
1765       j=j-1; 
1766     } 
1767     else if(getParentBit(i,j) == 2) { 
1768       i= i + 1; 
1769     } 
1770     else if(getParentBit(i,j) == 3) { 
1771       i= i - 1; 
1772     } 
1773   } 
1774   Serial.println("From Center to Start"); 
1775   Serial.println("##############"); 
1776   Serial.println(c); 
1777   return c; 
1778 } 
1779 
 
1780 void hanselAndGretel(int c) { 
1781   for(int i = 0; i < c; i++) { 
1782     Serial.println("In hansel and gretel"); 
1783     Serial.println(); 
1784     outputMaze(); 
1785     moveUnit(breadCrumbs[i]); 
1786   } 
1787 } 
1788 
 
1789 boolean foundShortest() { 
1790   for(int i = 0; i <= Size; i++) { 
1791     for(int j = 0; j <= Size; j++) { 
1792       if(E[i][j]) { 
1793         if(numberOfWalls(i,j) != 4) { 
1794           if(!getWallNorth(i,j)) { 
1795             if(j<7) { 
1796               if(!E[i][j+1]) { 
1797                 return false; 
1798               } 
1799             } 
1800           } 
1801           if(!getWallSouth(i,j)) { 
1802             if(j>1) { 
1803               if(!E[i][j-1]) { 
1804                 return false; 
1805               } 
1806             } 
1807           } 
1808           if(!getWallEast(i,j)) { 
1809             if(i<7) { 
1810               if(!E[i+1][j]) { 
1811                 return false; 
1812               } 
1813             } 
1814           } 
1815           if(!getWallWest(i,j)) { 
1816             if(j>1) { 
1817               if(!E[i-1][j]) { 
1818                 return false; 
1819               } 
1820             } 
1821           } 
1822         } 
1823       } 
1824     } 
1825   } 
1826   return true; 
1827 } 
1828 
 
1829 void speedRacer() { 
1830   int g = ((Size+1)^2)+1; 
1831   int cx = ((Size+1)^2)+1; 
1832   int cy = ((Size+1)^2)+1; 
1833   /* 
1834    
1835     
1836    Broken needs rewritting 
1837     
1838     
1839    */ 
1840   if(I[(int)ceil(((double)Size)/2)][(int)ceil(((double)Size)/2)] < g) { 
1841     g = I[(int)ceil(((double)Size)/2)][(int)ceil(((double)Size)/2)]; 
1842     cx = (int)ceil(((double)Size)/2); 
1843     cy = (int)ceil(((double)Size)/2); 
1844   } 
1845   if(I[(int)floor(((double)Size)/2)][(int)ceil(((double)Size)/2)] < g) { 
1846     g = I[(int)floor(((double)Size)/2)][(int)ceil(((double)Size)/2)]; 
1847     cx = (int)floor(((double)Size)/2); 
1848     cy = (int)ceil(((double)Size)/2); 
1849   } 
1850   if(I[(int)floor(((double)Size)/2)][(int)floor(((double)Size)/2)] < g) { 
1851     g = I[(int)floor(((double)Size)/2)][(int)floor(((double)Size)/2)]; 
1852     cx = (int)floor(((double)Size)/2); 
1853     cy = (int)floor(((double)Size)/2); 
1854   } 
1855   if(I[(int)ceil(((double)Size)/2)][(int)floor(((double)Size)/2)] < g) { 
1856     g = I[(int)ceil(((double)Size)/2)][(int)floor(((double)Size)/2)]; 
1857     cx = (int)ceil(((double)Size)/2); 
1858     cy = (int)floor(((double)Size)/2); 
1859   } 
1860   int c = goFromStartToSquare(cx,cy); 
1861   while(true) { 
1862     hanselAndGretel(c); 
1863     while(I[Lx][Ly] != 1) { 
1864       moveUnit(P[Lx][Ly]); 
1865     } 
1866   } 
1867 } 
1868 
 
1869 
 
1870 void outputMaze() {  
1871   for(int j=testSize;j>=0;j--) { 
1872     for (int i=0;i<=testSize;i++) { 
1873       Serial.print("#"); 
1874       if((Wn[i][j])) { 
1875         if(getWallNorth(i,j)) { 
1876           Serial.print("#####"); 
1877         }  
1878         else { 
1879           Serial.print("     ");  
1880         } 
1881 
 
1882         //virtual walls 
1883       }  
1884       else if(vWn[i][j]) { 
1885         if(getWallNorth(i,j)) { 
1886           Serial.print("@@@@@"); 
1887         }  
1888         else { 
1889           Serial.print("     ");  
1890         } 
1891 
 
1892       } 
1893       else { 
1894         Serial.print("     "); 
1895       } 
1896     }  
1897     Serial.println("#"); 
1898 
 
1899     for(int k=0; k<=2;k++) { 
1900       Serial.print("#"); 
1901       for (int i=0;i<=testSize;i++) { 
1902         if (getWallEast(i,j)&&(We[i][j])) { 
1903           if(k==1) { 
1904             if(I[i][j] > 9){ 
1905               Serial.print(" "); 
1906               Serial.print(I[i][j]); 
1907               Serial.print("  #"); 
1908             } 
1909             else{ 
1910               Serial.print("  "); 
1911               Serial.print(I[i][j]); 
1912               Serial.print("  #"); 
1913             } 
1914           } 
1915           else if(((k==0)&&(i==Lx))&&(j==Ly)) { 
1916             Serial.print(" $$$ #"); 
1917           }  
1918           else if((k==2)&&(E[i][j])) { 
1919             Serial.print("    *#");  
1920           } 
1921           else { 
1922             Serial.print("     #"); 
1923           } 
1924         }  
1925         else if (getWallEast(i,j)&&vWe[i][j]) { 
1926           if(k==1) { 
1927             if(I[i][j] > 9){ 
1928               Serial.print(" "); 
1929               Serial.print(I[i][j]); 
1930               Serial.print("  @"); 
1931             } 
1932             else{ 
1933               Serial.print("  "); 
1934               Serial.print(I[i][j]); 
1935               Serial.print("  @"); 
1936             } 
1937           } 
1938           else if(((k==0)&&(i==Lx))&&(j==Ly)) { 
1939             Serial.print(" $$$ @"); 
1940           }  
1941           else if((k==2)&&(E[i][j])) { 
1942             Serial.print("    *@");  
1943           } 
1944           else { 
1945             Serial.print("     @"); 
1946           } 
1947         } 
1948 
 
1949         else { 
1950           if(k==1) { 
1951             if(I[i][j] > 9){ 
1952               Serial.print(" "); 
1953               Serial.print(I[i][j]); 
1954               Serial.print("   "); 
1955             } 
1956             else{ 
1957               Serial.print("  "); 
1958               Serial.print(I[i][j]); 
1959               Serial.print("   "); 
1960             } 
1961           } 
1962           else if(((k==0)&&(i==Lx))&&(j==Ly)) { 
1963             Serial.print(" $$$  "); 
1964           }  
1965           else if((k==2)&&E[i][j]) { 
1966             Serial.print("    * ");  
1967           } 
1968           else { 
1969             Serial.print("      "); 
1970           } 
1971         } 
1972       } 
1973       Serial.println(); 
1974     } 
1975   } 
1976   for(int i=0;i<=testSize;i++){ 
1977     Serial.print("######");  
1978   } 
1979   Serial.println("#"); 
1980   Serial.println(); 
1981 } 
