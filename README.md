# Mobile6TiSCH: an OMNeT++ simulator for 6TiSCH-based networks with mobile nodes

## Description

The 6TiSCH architecture, defined by the IETF, is aimed at enabling the Industrial Internet of Things (IIoT).  
Many industrial applications involve mobile devices (e.g., mobile robots or wearable devices carried by workers), however 6TiSCH does not provide any mechanism to manage
node mobility.  

<img width="860" alt="image" src="https://user-images.githubusercontent.com/18681585/191436422-638e59a3-ae1e-4cc8-90c3-c01bcccd8dc2.png">

Here we present Mobile6TiSCH, a simulation tool based on OMNeT++, that implements a flexible and adaptive architecture to handle nodes' mobility in 6TiSCH networks. 
The proposed tool is modular, as showm in Figure, and allows to evaluate the Quality of Service (QoS) for each mobile node, in different scenarios considering different mobility patterns, scheduling algorithms, network configuration
parameters and Border Router deployment strategies. 

Mobile6TiSCH can be used to evaluate different solutions in a simulated environment before implementing them in practice.

<img width="949" alt="image" src="https://user-images.githubusercontent.com/18681585/191434837-291830a6-bc48-410d-acb7-37f09ad29a18.png">


## Migration from Mobile6TiSCH-legacy

Mobile6TiSCH is a revised version of Mobile6TiSCH-legacy, which is still available at https://github.com/marcopettorali/Mobile6TiSCH/tree/legacy.
In particular, Mobile6TiSCH underwent a refactoring process to make it more modular and extensible, and the codebase and the modules organization has been changed to make the code more readable and easier to maintain

The main differences between the two versions are:
- The **6TiSCH** module has been removed, and the module **IEEE802154TSCH** has been added
- Parts of the **Application** functionalities have been moved to the **MNMiddlewareBase** module
- **NCCore**, **BRCore** have been renamed to **NCMiddlewareBase** and **BRMiddlewareBase** respectively
- The metrics are collected by the **StatsCollector** module, to provide a more flexibility in the metrics collection


## Citing

If you use this simulator in your research, please cite our paper:

M. Pettorali, F. Righetti, C. Vallati, S. K. Das and G. Anastasi, "LASA: Location-Aware Scheduling Algorithm in Industrial IoT Networks with Mobile Nodes," 2023 IEEE 24th International Symposium on a World of Wireless, Mobile and Multimedia Networks (WoWMoM), Boston, MA, USA, 2023, pp. 185-194, doi: 10.1109/WoWMoM57956.2023.00033

or use the following BibTeX entry:

```
@INPROCEEDINGS{10195811,
  author={Pettorali, Marco and Righetti, Francesca and Vallati, Carlo and Das, Sajal K. and Anastasi, Giuseppe},
  booktitle={2023 IEEE 24th International Symposium on a World of Wireless, Mobile and Multimedia Networks (WoWMoM)}, 
  title={LASA: Location-Aware Scheduling Algorithm in Industrial IoT Networks with Mobile Nodes}, 
  year={2023},
  volume={},
  number={},
  pages={185-194},
  doi={10.1109/WoWMoM57956.2023.00033}}
```
