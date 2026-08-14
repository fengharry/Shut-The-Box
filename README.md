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

This is an in-depth compilation of results involving the board game "Shut the Box".

The general rules for the game are as follows:
* The board consists of numbers from 1-9 (I call these tiles)
* At the start of the game, all tiles are "face-up", and to win, the player must flip all of these tiles "face-down"
* Each turn, the player rolls two standard six-sided dice
* Then, the player flips down tiles that sum to the total value rolled by the two dice
* If the player cannot flip down any more tiles, the player loses
* The score of the game is calculated by the sum of the face-up tiles at the end of the game (win = 0)
* If the board only has Tile 1 on it, then the player will roll a single die for turn instead of 2

However, there are also several different variants to this game, some of which are described below:
* The board consists of different numbers (ex. 1-10, 1-12, etc.)
* The player rolls a single die for turn once their score is 6 or below
* The player isn't rolling two D6's, but rather one D12 (or other die variations)

This repository is dedicated to compiling the results of every single position of virtually every single Shut the Box variant, including results using the most optimal strategy and results using other common strategies.

For each position we will store (in /results):
* The probability of victory
* The average final score
* Whether the position can be reached by following the strategy
* The next tiles to be flipped for every roll
* The probabilities of winning for every roll
* The average score for every roll

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

[C++](Cplusplus-url)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

 * [C++](Cplusplus-url) is installed.
 * VSCode is configured with C++ (optional).

### Installation + Running

  To clone this project,
   ```
   git clone https://github.com/fengharry/Shut-The-Box.git
   ```
   To run the default program in main.cpp,
   ```
   cd build
   cmake ..
   cmake --build .
   ./main
   ```
   Other command line arguments may be added in a future update.
    
<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ROADMAP
## Roadmap

See the [open issues](https://github.com/fengharry/Shut-The-Box/issues) for a full list of proposed features. -->

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
