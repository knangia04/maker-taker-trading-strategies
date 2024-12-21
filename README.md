# group_02_project

## Project Overview:

### Summary:

#### Taker Strategy Overview:
This strategy uses a 100-day Simple Moving Average (SMA) to identify support and resistance levels for trading SPY (S&P 500 ETF).
Buy Signal: Triggered when the price stays above the SMA for two consecutive days with upward momentum.
Sell Signal: Triggered when the price stays below the SMA for two consecutive days with downward momentum.
Currently, the strategy uses all available capital for trades, but future improvements will incorporate risk management techniques like the Kelly Criterion. Initial testing was done in Jupyter Notebook, followed by rigorously backtest in Strategy Studio.

#### Maker Strategy Overview: 
Market-making strategies aim to provide liquidity to financial markets while profiting from bid-ask spreads and price fluctuations. One of the most popular approaches in this domain is grid trading, which excels in highly volatile, range-bound markets. By creating a "grid" of predefined buy and sell orders around the current price, the strategy systematically captures profits as prices oscillate between these levels.
Grid trading differs from trend-following strategies by focusing on market fluctuations rather than directional predictions. This makes it particularly effective in environments like SPY (S&P 500 ETF), where prices often move within a stable range.

## Getting Started and System Setup

#### Cloning the Repo:

```
git clone https://gitlab.engr.illinois.edu/fin556_algo_market_micro_fall_2024/fin556_algo_market_micro_fall_2024_02/group_02_project.git
```

#### Anaconda Setup:

If you do not have Anaconda on your device, please install it using the following commands:

```
curl -O https://repo.anaconda.com/archive/Anaconda3-2024.10-1-Linux-x86_64.sh
bash ~/Anaconda3-2024.10-1-Linux-x86_64.sh
```

This may take some time...alternatively you could use Miniconda3

**Environment SetUp:**

**Note: Assuming SS is already setup on your system and running under path: /home/vagrant/ss**

Once you have Anaconda3 on your system and are inside the group02 repo, use the following command to setup the environment

```
TODO: Once Project is completed export the yaml/yml inside the repo
conda env create -f environment.yml
```

Alterntatively use **pypy3** 

## Understanding the Codebase

## Teammates

### Siddarth Aananth

**About Me:**

Hello, I am Siddarth Aananth, a senior year undergraduate at the University of Illinois, Urbana-Champaign, majoring in Computer Science and Statistics with a minor in Mathematics. I am currently exploring both the applied and theoretical dimensions of machine learning under the guidance of Prof. Vikram Adve and Prof. Arindam Banerjee. My current research focuses are: ML@Edge and Generalization Research

A sampling of my research work:
SSL Open World Object Detection (Accepted NeurIPS OWA): https://openreview.net/forum?id=1rs9HXmJUv
Analysis of Sharpness Aware Minimization (Generalization Research Initial Ideas): https://arxiv.org/abs/2411.01714

Balancing my academic research with practical experience, I've also worked as a Machine Learning Engineer at Rivian. Here, I've contributed to refining ADAS software models and deepening my understanding of ML infrastructure, gaining valuable insights into product development in the electric vehicle industry and the role of AI in transforming it.

**LinkedIn:** https://www.linkedin.com/in/siddarthaananth/

**Other:** https://scholar.google.com/citations?user=R8vrP5gAAAAJ&hl=en

### Asuthosh Anandaram 

**About Me:**

Asuthosh, a senior at the University of Illinois at Urbana-Champaign, is set to graduate in 2025 with a major in Computer Engineering from the Grainger College of Engineering. He has a strong passion for systems programming, operating system design, and kernel development. His experience includes developing a custom real-time static and dynamic scheduler, a virtual memory page fault profiler, a Linux-based operating system, and an eBPF kernel module using Rust. Currently, he is diving into Windows kernel programming, further honing his expertise in low-level system development.
Asuthosh has extensive experience working with eBPF (Extended Berkeley Packet Filter), leveraging it for efficient and scalable performance monitoring and security analysis. He has designed and implemented custom eBPF programs for tracing, packet filtering, and network traffic analysis.



**LinkedIn:**

**Other:**

### Khushi Kalra

**About Me:**

**LinkedIn:**

**Other:**

### Krish Nangia

**About Me:** Hello, my name is Krish Nangia, and I am currently an undergraduate junior majoring in Computer Engineering at the Univesity of Illinois at Urbana-Champaign, with an expected graduation date of December 2025. I am a driven student with a diverse background in machine learning, data science, and software engineering. My experience spans various industries, including insurance, agriculture, and renewable energy, where I’ve delivered impactful projects, such as deploying scalable LLM solutions, creating time-series forecasting models, and developing IoT-based tools. 

My technical toolkit includes proficiency in programming languages such as Java, Python, C, C++, and more. I have explored the world of machine learning with frameworks like TensorFlow, PyTorch, and Scikit-Learn, and also have harnessed the power of data science libraries like Pandas, Matplotlib, and Numpy. Additionally, I have knowledge in SQL and web frameworks like Flask.

​I am very passionate about leveraging technology for the betterment of today's society. I hope to explore different aspects of software development and data science/analytics to ultimately use the skills gained to help further advance technology in other fields.

**LinkedIn:** https://www.linkedin.com/in/krish-nangia-uiuc

**Other:** https://www.github.com/knangia04

___


