Linear Assignment Problem solver using kNN-based Jonker-Volgenant algorithm
==================================================================

Codes for kNN-based Jonker-Volgenant algorithm described in our paper ["OoDAnalyzer: Interactive Analysis of
Out-of-Distribution Samples"](https://ieeexplore.ieee.org/document/8994105) (TVCG 2020).

Installing
----------
```
pip3 install fastlapjv
```
Tested on Windows, and Linux with python 3.6.

Usage Example
-----
Run this example to compare our method and the eaxct JV algorithm. Runing this example requires the installation of [scipy](https://www.scipy.org/) and [lapjv](https://github.com/src-d/lapjv).
Note: the latest [lapjv](https://github.com/src-d/lapjv) uses some new features of python 3.8 or higher, and [fastlapjv](https://github.com/thu-vis/fast-lapjv) does not support python 3.8 or higher now. If you want to compare fastlapjv and lapjv, please use version 1.3.1 of lapjv, which support python 3.6.
```
python test/test.py
```

## Acknowledgement
This code is implemented based on the published code of [lapjv](https://github.com/src-d/lapjv), and it is our pleasure to acknowledge their contributions.

## Citation
If you use this code for your research, please consider citing:
```
@article{chen2020oodanalyzer,
    title = {{OoDAnalyzer}: Interactive Analysis of
Out-of-Distribution Samples},
    author = {Chen, Changjian and Yuan, Jun and Lu, Yafeng and Liu, Yang and Yuan, Songtao and Liu, Shixia},
    journal = {IEEE Transactions on Visualization and Computer Graphics (accepted)},
    year = {2020}
}
```

## Contact
If you have any problem about our code, feel free to contact
- ccj17@mails.tsinghua.edu.cn

or describe your problem in Issues.
