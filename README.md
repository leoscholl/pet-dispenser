# pet-dispenser
A treat dispenser can be a useful training tool and a fun game for your pet. There are several [commercially](https://www.amazon.com/PetSafe-Treat-Remote-Reward-Trainer/dp/B0010B8CHG/ref=asc_df_B0010B8CHG) [available](https://clever.pet/)
treat dispensers and [even](https://blog.arduino.cc/2017/01/25/build-an-automatic-cat-treat-dispenser-with-hummingbird/) [more](https://www.instructables.com/id/Arduino-controled-dog-foodtreat-dispenser/) [guides](https://create.arduino.cc/projecthub/isaackoz/dog-treat-dispenser-using-cayenne-991222) [online](https://www.instructables.com/id/IoT-Treat-Dispenser-for-Pets/) describing do-it-yourself pet feeders and treat dispensers. However, the commercial products are expensive and closed-source, and the DIY solutions are neither well documented nor adequately tested, leaving too much unknown to warrant spending time and money to build and troubleshoot a homemade device. 

![my dog using the device](https://github.com/leoscholl/pet-dispenser/raw/master/video.gif)

Fortunately, [this paper](https://doi.org/10.1016/j.ohx.2016.09.001) describes and tests a robust, open-source feeder with design files and sample code. The only problem is that the authors use laser-cut acrylic to fabricate their device, which requires access to expensive machinery. This seemed unnecessary. I looked around the house and found a salsa jar and some cardboard tube and made a low-cost version of their device.

![the dispenser](https://github.com/leoscholl/pet-dispenser/raw/master/front.JPG)

This prototype dispenser is made mostly from cardboard and plastic sheets from a tupperware box with a lot of hot glue. The lid of the salsa jar has a cutout for treats at the top and a hole in the middle for the servo. I used a small piece of MDF to make the notched wheel and an IR photodiode and sensor to detect successful release of pellets.

![the buttons](https://github.com/leoscholl/pet-dispenser/raw/master/button.JPG)

The buttons are two pieces of plastic sandwiched over a small tactile switch. The whole thing runs on a 5v arduino pro mini with the example sketch from this repository. Several modes are made available:
1. Any-button: pulling any of the input pins to ground will dispense food
2. Alternating: triggering a button only works once in a row
3. Unbalanced: 1st button releases more treats than the others
4. Multi-press: two or more presses are required to dispense a treat (my dog can't learn this one!)
