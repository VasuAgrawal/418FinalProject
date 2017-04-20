## Summary

We are going to implement a black box optimizer which uses genetic programming
to evolve a set of parameters for an arbitrary program. Given a user-provided
evaluator and set of parameters, our framework will attempt to determine an
optimal configuration.

## Background

A [genetic algorithm](https://en.wikipedia.org/wiki/Genetic_algorithm) is one
which optimizes a set of parameters in a process inspired by natural selection.
An initial population of parameter sets ("organisms") is randomly generated, and
each is evaluated for fitness. The most fit individuals are selected to "breed",
and a new generation is created from combinations of the selected individuals.
This process is repeated for many generations until improvement stagnates.

![Genetic algorithm flow-chart](./genetic-algorithm.png)

Pseudo-code for a genetic algorithm could be as follows:

```
population = generate_random_organisms()
for i = 1 to num_generations
    for organism in population:
        evaluate_fitness(organism)
    remove_weak_organisms(population)
    new_population = {}
    for pair of organisms in population:
        new_population.add(breed(pair))
    population = new_population
```

Genetic algorithms allow for optimizations on fitness functions that would be
hard, or even impossible, to optimize using more standard techniques. For
example, many machine learning techniques require differentiating the fitness
function with respect to the input parameters. But, if the fitness evaluator
performs a long simulation, differentiating would be very difficult.

There are many avenues for parallelization in a genetic optimizer. Each organism
is evaluated for fitness independently of the others, allowing for parallelism
across evaluations. Additionally, the culling and breeding procedures can be
performed in parallel.

Our optimizer will expose a command-line interface to users, from which they
will be able to submit a configuration file and a working program (either
source code or binary) that will act as the fitness evaluator. The
configuration file will define the parameters of the simulation (generation
size, number of generations, etc.) and organism parameters, including valid
ranges for these parameters. For example:

```
{
  "cmd": "./fitness.py",
  "generation_count": 1000,
  "generation_size": 1000,
  "parameters": [
    { "name": "length",
      "min": 1,
      "max": 100
    },
    { "name": "width",
      "min": 1,
      "max": 50
    }
  ]
}
```

To invoke the optimizer, the user would use the command

```distopt config.json```

The optimizer will then, for a given parameter set, invoke the command

```./fitness.py --length={length} --width={width}```

to determine its fitness, which `fitness.py` outputs to STDOUT as an integer
value, with higher values denoting better fitness of the given parameters.

## The Challenge

One of our goals includes making the framework compatible with many languages.
In order to achieve this goal and require minimal modification by the user to
actually use our framework, we need to find an easy, language-agnostic way of
passing in data to a program. Similarly, we need to develop our framework to be
able to run any kind of evaluator written in any language in order to achieve
maximum usability. This forces a certain degree of separation between the
framework and the program which will make it more difficult to optimize for
performance as much of the executed work is beyond our control. 

One solution to this performance problem is to add more machines. However, that
poses additional problems as we now need to find a way to ensure that the user
provided evaluator will run smoothly on all workers rather than just a single
one. This may include installing additional dependencies. At the very least, the
framework will now be responsible for scheduling and distributing parameter
evolutions across different machines, and dealing with all of the communication
challenges and overhead that are associated with something of this nature.

Our framework needs to be able to optimize the given parameters without any
knowledge of the execution properties, memory characteristics, or I/O properties
of the evaluator. This means that our framework must be performant without any
initial knowledge of the nature of the workload. Additional information may be
requested from the user to attempt to determine the workload parameters in order
to efficiently schedule multiple evaluators.

## Resources

Our final project will be designed to work across a wide range of computers,
from a single process to multiple workers in a cluster. As such, we'll be able
to begin developement on our personal machines, and will later be able to scale
up to a cluster to test performance. We hope that our level of access on the GHC
machines makes it possible to configure them as a cluster, but should that fail
we would require elevated access or access to some other computer cluster where
we're able to configure the network (e.g. hosts files) as necessary. We hope to
be able to design the system to fully utilize a heterogenous cluster, but will
begin by targeting a standard cluster of reasonably powerful (e.g. GHC) machines
for simplicity.

For the framework itself, we plan on starting from a clean slate. We are leaning
towards using Go, which has its own RPC system to make using multiple workers
easy. We want to figure out as much of this on our own as possible before
potentially exploring the literature to determine improvements we can make to
our framework for usability and performance.

## Goals & Deliverables

We will consider our project successful when we are able to take an arbitrary
program (runnable by default on our worker machines) and apply genetic
programming to evolve and optimize parameters across multiple workers. We hope
to see close to linear speedup when adding additional worker nodes as there is
no inherent communication between instances of each evaluator. Communication
overhead is expected to be negligible compared to the cost of running each
evaluator instance and can ideally be ignored entirely, since we are expecting
to see relatively expensive evaluators.

Once we have our target implementation working, we have a few potential stretch
goals in mind depending on which aspect of our project we find to be the most
intriguing:

* Virtualize each instance of the evaluator
* Determine evaluator workload profile and use information to reschedule
* Optimize for performance on heterogenous clusters (a la BOINC)

## Platform

Our choice of a networked cluster of reasonably fast computers makes sense for
this application as it's the de facto standard for available compute at large
companies (see Google, Amazon, Microsoft). We believe this is reasonable because
companies of this scale are also most likely to be the ones performing research
which requires this sort of black box optimization. That said, we hope to design
the system to work across large numbers of heterogenous machines to appeal to
all levels of compute capability.

## Schedule

1. **Mon April 17th (11:59 pm):** By this point, we want to be able to evaluate
   parameters in the arbitrary user-specified program. We further want to use
   the results to implement some basic genetic programming algorithm to tune at
   least a single parameter.
1. **Tue April 25th (11:59 pm):** Project checkpoint report due. By this point,
   we want to be able to employ genetic programming to optimize some reasonable
   number of parameters for an arbitrary program on a single machine.
1. **Tue May 2nd (11:59 pm):** By this point, we want to be able to have our
   framework schedule invocations of the user's evaluator across multiple
   machines for performance. We also plan on taking performance measurements to
   determine the relative utility of adding more workers.
1. **Wed May 10th (9:00 am):** Project pages made available. We plan to have
   spent the last week of our project tidying up any loose ends in order to make
   the project more useable. Once that is completed, we plan on working on our
   stretch goals, such as dynamic profiling of the user's application to
   generate information to be used in rescheduling for maximum performance.
1. **Thu May 11th (3:00 pm):** Finalists announced, presentation time sign ups.
1. **Fri May 12th (11:59 pm):** Final report due, parallelism competition.

## Authors

Developed by [Vasu Agrawal](https://github.com/VasuAgrawal) and [Lukas Peraza](https://github.com/LBPeraza).

<!--## Welcome to GitHub Pages-->

<!--You can use the [editor on GitHub](https://github.com/VasuAgrawal/418FinalProject/edit/master/README.md) to maintain and preview the content for your website in Markdown files.-->

<!--Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.-->

<!--### Markdown-->

<!--Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for-->

<!--```markdown-->
<!--Syntax highlighted code block-->

<!--# Header 1-->
<!--## Header 2-->
<!--### Header 3-->

<!--- Bulleted-->
<!--- List-->

<!--1. Numbered-->
<!--2. List-->

<!--**Bold** and _Italic_ and `Code` text-->

<!--[Link](url) and ![Image](src)-->
<!--```-->

<!--For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).-->

<!--### Jekyll Themes-->

<!--Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/VasuAgrawal/418FinalProject/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.-->

<!--### Support or Contact-->

<!--Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.-->
