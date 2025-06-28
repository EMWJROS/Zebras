# Zebras
A program that helps you solve military IQ tests and others
![zebra](https://github.com/user-attachments/assets/95141511-be0a-4d21-8e9c-9faeeb0e7e72)

### Background
In 1989 as I was doing my military service in the Swedish
Air Force, we had a competition between the different platoons. It
mostly consisted of various track and fields events, but they had also
included an intellectual event. They said it was an IQ test and since
the other soldiers knew I was a physics student, my name was immediately
put forward. I didn't object. In high school, I had read Eysenck's *Know
Your Own IQ* and had done fairly well on those tests, so I wasn't worried (the book was old
and I hadn't heard about the Flynn Effect). It was
a bigger problem to come up with a second name, since we needed two
representatives from our platoon, but finally an electrician from Skövde
(a small town in a rural area) volunteered. On the day of the
tournament, we sat down in a class room and were given something that didn't
resemble Eysenck's test at all but was something which is known as *The
Zebra Puzzle* (se below). I had actually come across this too in high school. It
had been published in a rather sensationalist popular science magazine
called *Illustrerad Vetenskap*. It was reminiscent, but more
complicated, of some simple logical puzzles that I had seen in the
*Fantomen* (The Phantom) superhero comic when I was a kid. (My mother
thought *Fantomen* was too violent so I couldn't read it at home, but
friends with more liberal parents had it and also the doctor's waiting
room where we used to go.) The Fantomen puzzles were crime solving
puzzles where a detective was taking down testimonies from witnesses and
by logical deduction you could find out who the culprit was. I knew that
the Fantomen puzzles could be solved by marking true and false in a
grid. I also remembered from high school that the Zebra Puzzle could be
solved without a grid and by just using the given information in the
right order. Since I wasn't 100 % sure that it was the same problem as
in high school and also since I don't perform well under stress, I
thought it better to do the systematic way with the grids. The problem
was that there were five houses, five nationalities, etc, that made the
grid five dimensional! The Fantomen problems had only been
three-dimensional but they were possible to solve by using 2D slices
through the grid and I thought something similar would work with higher
dimensions. So I started marking connections between the information
titbits that were given, but I almost needed a different 2D slice for
each new information item. We were given note paper and I had to take a
lot but got nowhere. Midway, the electrician stood up and handed in his
paper. I was thinking that this had been to much for his poor
electrician's brain and he had given up. I struggled to the bitter end
but couldn't complete it. When I got out, the other soldiers were
waiting outside and asked me how it went. &mdash; "I'm not sure," I said,
although I was pretty sure that I had failed. I, in my turn, asked the
electrician how he had done and he claimed that he had solved it, but I
didn't believe him. Later the results were announced and he really was
correct. My platoon mates were, however, very tactful and not a single
one asked me why I had failed or brought up the topic ever again. Maybe
they had a lot of experience in failing tests and didn't think that it
was strange \... I honestly didn't care because it wasn't a real IQ test
and besides I had already solved it in high school \... I felt a little
bad about letting the platoon down, though, especially since they had
such high expectations for me.

### The famous Zebra Puzzle

> 1. There are five houses.
> 1. The Englishman lives in the red house.
> 1. The Spaniard owns the dog.
> 1. Coffee is drunk in the green house.
> 1. The Ukrainian drinks tea.
> 1. The green house is immediately to the right of the ivory house.
> 1. The Old Gold smoker owns snails.
> 1. Kools are smoked in the yellow house.
> 1. Milk is drunk in the middle house.
> 1. The Norwegian lives in the first house.
> 1. The man who smokes Chesterfields lives in the house next to the man with the fox.
> 1. Kools are smoked in the house next to the house where the horse is kept.
> 1. The Lucky Strike smoker drinks orange juice.
> 1. The Japanese smokes Parliaments.
> 1. The Norwegian lives next to the blue house.
>
> Now, who drinks water? Who owns the zebra?
>
>In the interest of clarity, it must be added that each of the five houses is painted a different color, and their inhabitants are of different national extractions, own different pets, drink different beverages and smoke different brands of American cigarets. One other thing: in statement 6, right means your right.

### The current program (and its humble beginnings)
In 2005 I started buying a magazine called *Hjärngymnastik* (roughly
"Brain teasers"). In it they had similar problems as the Zebra problem
and I wrote a c++ program to solve them. The program presented here is
an improved version of that program. This type of problem is apparently
called a *Constraint Satisfaction Problem*, so the program consists of a
class CspSolver. You instantiate it by giving the number of categories
and properties. In the Zebra problem we have 5 categories (5 houses) and
6 properties (house number, house colour, nationality, pet, beverage,
cigarette brand). (The house number may not be obvious, but I think it's
needed to code some of the clues.) Under the hood, the program creates a
6-dimensional table with five values in each dimension. In our case, it
will hold 15,625 Boolean values. (It is actually implemented as a linear
array, in order to make the program independent of the chosen
dimensions.) This is what I tried to do with note paper! All boxes of
the truth table are true when we start. You then need to code the clues.
This is done by creating a number of fact_items. These can be regarded
as the atoms by which we build clues, like "Englishman", "horse" and
"red". The first real clue, is that the Englishman lives in the red
house. We then tell the CspSolver this by running `connect()` on the
fact_items representing Englishman and red. Under the hood, the solver
now goes through the truth table and marks all values (2500 of them)
where Englishman is combined with another colour than red, and all
values where red is combined with another nationality than English
(another 2500), as false. There are ten clues like this one that we can
call *individual clues*, since they tell us something about one
individual. Then there are five clues that we can call *relational
clues*, since they tell us something about the relations, such as "The
Norwegian lives next to the blue house". Indirectly, there is also an
individual clue hidden in this, since it tells us that the Norwegian
doesn't live in the blue house. We will treat the relational clues
later, but we can give this piece of information to the solver by
running `disconnect()` on the fact_items representing the Norwegian and
the blue colour. The solver now goes through all cells in the truth
table that have nationality = Norwegian and colour = blue (625 of them)
and mark them as false. By doing this for the ten individual clues and
the four relational clues that aren't obvious, we can retrieve the
remaining possible combinations by running `get_possibilities()`. It turns
out that the initially 15,625 possible combinations have been reduced to
a mere 110. I call these *factoids*, and they describe possible
combinations of properties that are consistent with the clues given so
far. One states, e.g., that the Englishman lives in the red House 3 and
that he owns the zebra, that he drinks milk and smokes Chesterfields.
Another claims that the Spaniard lives in the green House 2 and has a
dog, drinks coffee and smokes Chesterfields. Obviously both can't be
true at the same time, since only one smokes Chesterfield. The problem
solving now proceeds by trying to find a combination of factoids where
each fact item is only used once. This is achieved by running
`get_unique_combos()`. In theory, there are 122,391,522 combinations to
check. (Again, the futility of me trying to do this with pen and paper
is so obvious.) This will leave us with a list of 924 factoid
combinations that will fulfil all the clues given so far. Now we loop
over these 924 and filter out the ones that fulfil the remaining five
relational clues. Luckily, only one is left, and thus we know that it is
our Japanese gentleman who owns the zebra and the Norwegian who is fond
of water.



| House | Nationality | Colour | Pet | Drink | Cigarette |
| -------- | ------- | ------- | ------- | ------- | ------- |
| 1 | Norwegian | yellow | fox | water | Kools |
| 2 | Ukranian | blue | horse | tea | Chesterfields |
| 3 | English  | red | snails | milk | Old Gold |
| 4 | Spanish | ivory | dog | orange juice | Lucky Strike|
| 5 | Japanese | green | zebra | coffee | Parliament|

![jap](https://github.com/user-attachments/assets/7bffe09f-9b4f-4812-8080-de14ce40b2fd)
![norw](https://github.com/user-attachments/assets/a438a908-62d4-4afd-a026-24a82a224fb9)

The Japanese taking his zebra for a ride around the neighbourhood
and the Norwegian enjoying a glass of his favourite drink, straight from
the source it would seem[^1].

### Suggestions for improvements:

-   Use unique_ptr instead of raw pointers.

-   Change name of FactItem to FactAtom.

-   Factoid uses the internally created ID. It would be more natural to
    use the pointers that the client has received.

-   The name could then be stored in the FactItem (or FactAtom) and
    retrieved by a call on the object.

-   Maybe do the similar for FactCombo so it contains pointers to
    Factoids. It would be easier to use for the client.

[^1]: And I do like that he took the trouble to bring a proper drinking
    glass on his hike into the wilderness.
