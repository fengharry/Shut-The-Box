<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
<!-- [![Contributors][contributors-shield]][contributors-url] -->
[![LinkedIn][linkedin-shield]][linkedin-url]


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <!-- <li><a href="#usage">Usage</a></li> -->
    <!-- <li><a href="#roadmap">Roadmap</a></li> -->
    <!-- <li><a href="#contributing">Contributing</a></li> -->
    <li><a href="#contact">Contact</a></li>
    <!-- <li><a href="#acknowledgments">Acknowledgments</a></li> -->
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This is an in-depth compilation of results and programs involving the board game "Shut the Box".

As an example variant, the rules for the standard Shut The Box (size 9) are as follows:
* The board consists of numbers from 1-9 (I call these tiles)
* At the start of the game, all tiles are "face-up", and to win, the player must flip all of these tiles "face-down"
* Each turn, the player rolls two standard six-sided dice
* Then, the player flips down tiles that sum to the total value rolled by the two dice
* If the player cannot flip down any more tiles, the player loses
* The score of the game is calculated by the sum of the face-up tiles at the end of the game (win = 0)
* If the board does not have tiles 7, 8, and 9 still available, then the player can choose whether to roll a single dice or two.

However, there are also several different variants to this game, some of which are described below:
* The board consists of different numbers (ex. 1-10, 1-12, etc.)
* The player can only roll a single die for turn once their score is below a certain threshold
* The player isn't rolling two D6's, but rather one D12 (or other die variations)

As an aside, the variant I've played the most is one where the player cannot roll just a single die per turn unless their score is exactly 1. This also happens to be the default setting of the program.

This repository is dedicated to compiling the results of every single position of virtually every single Shut the Box variant, including results using the most optimal strategy and results using other common strategies.

### Run Types

#### Simulations

Simulates a number of games, each from start to finish, as if a real player was playing the game. 

The following data points are outputted:
* The number of games that were simulated
* How many games were won
* The overall win probability
* The average score 

#### Calculations

Calculates specific data points for every possible position (using recursion).

For each possible position, the following data points are outputted (into a csv file):
* The probability of victory
* The average final score
* Whether the position can be reached by following strategy X
* The next tiles that should be flipped for every next roll

For the initial position, the following data points are also outputted:
* The probabilities of winning for every next roll
* The average score for every next roll

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

[C++](Cplusplus-url),
[CMake](CMake-url)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

 * [C++](Cplusplus-url) is installed.
 * [CMake](CMake-url) is installed
 * VSCode is configured with C++ (optional).

### Installation + Running

  To clone this project,
   ```
   git clone https://github.com/fengharry/Shut-The-Box.git
   ```
   To build the program,
   ```
   cd build
   cmake ..
   cmake --build .
   ```
   To run the example program with tiles 1-9 (example9.cpp),
   ```
   ./example9
   ```
   To run a simulation of a game and a strategy,
   ```
   ./main -r SIMULATION -g [GAME_SIZE] -s [STRATEGY] -f [SIM_FILE]
   ```
   * GAME_SIZE: the maximum tile on the board (ex. GAME_SIZE=6 -> Tiles 1-6)
   * STRATEGY: the name of the strategy to be used (see List of Names)
   * SIM_FILE: the name of the file to print out the results (leave blank to output to the terminal)

   To run a calculation of a game and a strategy,
   ```
   ./main -r CALCULATION -g [GAME_SIZE] -s [STRATEGY] -c [CALC_CSV_FILE]
   ```
   * GAME_SIZE: the maximum tile on the board (ex. GAME_SIZE=6 -> Tiles 1-6)
   * STRATEGY: the name of the strategy to be used (see List of Names)
   * CALC_CSV_FILE: the name of the file to print out the results
   
    
<p align="right">(<a href="#readme-top">back to top</a>)</p>


## Roadmap

   The current program does not support all variants of Shut The Box. Below are some of the in progress variants being implemented:
   * The ability for the player to CHOOSE between rolling 1 or 2 dice for a turn
   * Rolling different-sided dice 
   * Setting the order of which dice to roll in specific scenarios
     * Scenario A: minimum score threshold
     * Scenario B: specific face-down (or face-up) numbers
     * Scenario C: Player choice

  Please note that the above variants, when implemented, might not be able to run with command-line arguments. For example, it is possible to set custom initial tiles and custom dice, but these are not possible to run through the command-line, or more specifically, main.cpp. These commands may be added in future updates, but it's not guaranteed.
   
   The variants that will NOT be covered are listed below:
   * Events occurring in one game being influenced by the events of a previous one
   * Characteristics/Rules of a variant changing from game to game
   * Scores/Tiles with non-integer values

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTRIBUTING
## Contributing

Any and all contributions you make would be **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "improvement".

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/something`)
3. Commit your Changes (`git commit -m 'Add something'`)
4. Push to the Branch (`git push origin feature/something`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p> -->



<!-- CONTACT -->
## Contact

Harry Feng - harryfeng07@gmail.com

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo_name.svg?style=for-the-badge
[contributors-url]: https://github.com/github_username/repo_name/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/github_username/repo_name.svg?style=for-the-badge
[forks-url]: https://github.com/github_username/repo_name/network/members
[stars-shield]: https://img.shields.io/github/stars/github_username/repo_name.svg?style=for-the-badge
[stars-url]: https://github.com/github_username/repo_name/stargazers
[issues-shield]: https://img.shields.io/github/issues/github_username/repo_name.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo_name/issues
[license-shield]: https://img.shields.io/github/license/github_username/repo_name.svg?style=for-the-badge
[license-url]: https://github.com/github_username/repo_name/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/harry-feng-0839b733b/
[product-screenshot]: images/screenshot.png
<!-- Shields.io badges. You can a comprehensive list with many more badges at: https://github.com/inttter/md-badges -->
[Cplusplus]: https://github.com/isocpp/logos/cpp_logo.png
[Cplusplus-url]: https://cplusplus.com/
[CMake-url]: https://cmake.org/
