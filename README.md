<h1>Sorting Parallelization via OpenMP with Solar Vehicle Applications</h1>
<h3>By Logan Richardson & Isaac Allen</h3>

<h3>Project Purpose:</h3>
<p>This project parallelizes the QuickSort and Merge Sort algorithms using OpenMP. Our goal was to practice parallelization and build a useful tool for team Sunergy. Team Sunergy is Appalachian State University's solar vehicle racing team. Data is collected from the battery management system at the race each year.
The data contains over thirty variables such as state of charge, highest/lowest cell volt, pack current charge limit, and highest/lowest cell temperature.
The battery management system of the car constantly outputs data and the dataset therefore scales quite rapidly.
For example, in the period from Wed Jul 21 17:34:07 PDT 2021 to Thu Jul 22 00:27:15 PDT 2021 (Roughly 7 Hours) the BMS generated 78,210 data points.
Our program parallelizes the sorting of this data using QuickSort & Merge Sort. The program scans in the comma separated value dataset and populates the data structure. We organized the data into a structure of structures grouping similar variables together. The bmsData structure contains pack, cell, and hiLo data sub structures. The end user can scan in a csv dataset and then choose which variable to sort the data upon.</p>

<h3>How to Use:</h3>
<strong>1. Setup:</strong> <t>Clone the repo on a server such as App State's student2 machine.<br>
<strong>2. Compile:</strong> <t>Run the 'make' command found within the makefile.<br>
<strong>3. Run:</strong> ./sorter -n {X} -t {Y} -- X = 2^# of arrays  -- Y = # of threads.<br>
<strong>4. Commands:</strong> -m runs MergeSort, -q runs QuickSort, ./sorter -h provides information.<br>

<h3>Completed objectives:</h3>
<ol>
   <ol>
     <li>Sequential & Parallelized QuickSort</li>
     <li>Sequential & Parallelized Merge Sort</li>
     <li>Speedup comparison between algorithms</li>
     <li>Created a tool which the solar vehicle team could use and expand upon</li>
     <li>Practiced parallelization</li>
     <li>Learned how to parse & import CSV data</li>
     <li>Learned more about valgrind and gdb</li>  
   </ol>
</ol>

<h3>Unfinished objectives:</h3>

<ol>
  <ol>
    <li><h4>Bug related:</h4></li>
       <ol>
         <li>GPU QuickSort</li>
       </ol>
    <li><h4>Time related:</h4></li>
       <ol>
         <li>Sorting the data on the varibale of choice.</li>
       </ol>
  </ol>
</ol>


---


<h3>Resource List:</h3>

1. <strong>"Data Collection and Analysis Techniques for Solar Car Telemetry Data"</strong><br>
~~~
https://solarcar.mst.edu/wp-content/uploads/sites/14/2019/10/Data-Collection-and-Analysis-Techniques-for-Solar-Car-Telemetry-Data.pdf
~~~

2. <strong>Sample Data Sets: data.world</strong><br>
~~~
https://data.world/datasets/solar
~~~

3. <strong>"Parallel Depth First Search, Part 1: Implementation"</strong><br>
~~~
https://www.lrde.epita.fr/~bleton/doc/parallel-depth-first-search.pdf
~~~

4. <strong>Section 3.2 of textbook: "Parallel Programming, Concepts and Practice"</strong><br>
By Bertil Schmidt, Jorge Gonazales-Dominguez, Christian Hundt, & Moritz Schlarb
~~~
Parallel Programming, Concepts and Practice
~~~

5. <strong>"How can I read and parse CSV files in C++?"</strong><br>
~~~
https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
~~~

6. <strong>"Quicksort: an improved GPU-based implementation of quicksort"</strong><br>
By Emanuele Manca, Andrea Manconi, Alessandro Orro, Giuliano Armano and Luciano Milanes.
~~~
Quicksort: an improved GPU-based implementation of quicksort
~~~

7. <strong>"GPU-Quicksort: A Practical Quicksort Algorithm for Graphics Processors"</strong><br>
By Daniel Cederman and Philippas Tsigas
~~~
GPU-Quicksort: A Practical Quicksort Algorithm for Graphics Processors
~~~

8. <strong>Advice and Guidance from Dr. Cindy Norris</strong><br>
Professor of Computer Science, Appalachian State University
~~~
https://compsci.appstate.edu/faculty-staff/dr-cindy-norris
~~~

9. <strong>Team Sunergy Private Dataset</strong><br>
cellvoltages_2021-07-21-17-34-00

---

<h3>Hours Log: Total Hours 87.57</h3>

| # Hours | Date / Time | In Class Yes/No | Description | Person |
| :----: | :----: | :----: | :----: |  :----: | 
| 4<br> (2 x 2) | 6/15/22<br> 2 - 4p | Y | Picking a topic & starting research | LWR & IMA |
| 1.5<br> | 6/16/22<br> 11 - 12:30p | N | Reading about GPU Quicksort | IMA |
| 1<br> | 6/16/22<br> 1 - 2p | N | Article Reading | LWR |
| 5<br> (2.5 x 2) | 6/16/22<br> 2 - 4:30p | Y | Read QuickSort article, setup codebase, gpuquicksort.cu & makefile | LWR & IMA |
| 2<br> | 6/17/22<br> 2 - 4p | N | Debugging makefile, working on Gpuquicksort, organizing codebase | LWR| 
| 2.75<br> | 6/17/22<br> 2:30 - 5:15p| N |Structuring the build files and layout of the codebase |IMA |
| 1<br> | 6/17//22<br> 8 - 9p | N | Reading Quicksort documentation | LWR |
| 5.32<br> (2.66 x 2) | 6/18/22<br> 8 - 10:40p | N | Implementing Host logic  | LWR & IMA |
| 6<br> (3 X 2) | 6/20/22<br> 9:30 - 12:30p | N | CPU Kernel implementation & consulting with Dr. Norris | LWR & IMA |
| 5<br> (2.5 x 2) | 6/20/22<br> 2 - 4:30p | Y | Research, consulting with Dr. Norris, GPUQuickSort | LWR & IMA |
| 8<br> (4 x 2) | 6/20/22<br> 8:30 - 12:30a | N | BMS Data entry pair programming session | LWR & IMA |
| 2<br> (1 x 2) | 6/21/22<br> 10 - 12p | N |Debugging sorter.cu | LWR|
| 3<br> (1.5 x 2) | 6/21/22<br> 12:30 - 2p  | N | Debugging code | LWR & IMA |
| 5<br> (2.5 x 2) | 6/21/22<br> 2 - 4:30p | Y | Code Compiling, Further Debugging | LWR & IMA |
| 0.75<br> | 6/21/22<br> 4:30 - 5:15p | N | Debugging | IMA |
| 6<br> (3 x 2) | 6/21/22<br> 9:45 - 12:45a | N | Resolved Execution Bugs | LWR & IMA |
| 1.75<br> | 6/22/22<br> 7 - 8:45a | N | Optimizing Memory Access | IMA |
| 0.5<br> | 6/22/22<br> 9 - 9:30a | N | Further Investigations | IMA |
| 6<br> (3 x 2) | 6/22/22<br> 10 - 1p | N | Implementing Algorithm, debugging, documenting code, updating readme, & creating power point presentation. | LWR & IMA |
| 5<br> (2.5 x 2) | 6/22/22<br> 2 - 4:30p | Y | Work on presentation, redesign git repo, rework readme, mergesort is now running | LWR & IMA |
| 8<br> (4 x 2) | 6/22/22<br> 5 - 9p | N | Creating presentation, polishing readme, debugging sorting algorithms to increase speedup. | LWR & IMA |
| 8<br> (4 x 2) | 6/23/22<br> 9:30 - 1:30p | N | Refactored algorithms, fixed speedup issue, created sorted CSV download, polished presentation & readme | LWR & IMA |

-----
