# # retransmissions ON
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c sfcomparison_retron_sddu4_convergecast -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c sfcomparison_retron_orchestra_convergecast -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c sfcomparison_retron_amus_convergecast -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c sfcomparison_retron_alice_convergecast -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c sfcomparison_retron_dddu_reqres -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c sfcomparison_retron_orchestra_reqres -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c sfcomparison_retron_amus_reqres -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c sfcomparison_retron_alice_reqres -n ../src:. omnetpp.ini

# # perturbation
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_convergecast_p75_pert0 -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_convergecast_p75_pert1 -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_convergecast_p75_pert5 -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_convergecast_p75_pert10 -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_reqres_p75_pert0 -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_reqres_p75_pert1 -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_reqres_p75_pert5 -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_reqres_p75_pert10 -n ../src:. omnetpp.ini

# # scenario
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_convergecast_p75_tfpd_11 -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_reqres_p75_tfpd_11 -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_convergecast_p75_tfpd_18 -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_reqres_p75_tfpd_18 -n ../src:. omnetpp.ini

# # batch
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_convergecast_p75_batch -n ../src:. omnetpp.ini
# opp_runall -j 24 ../src/Mobile6TiSCH-3-0-Chester -m -u Cmdenv -c mobilityanalysis_reqres_p75_batch -n ../src:. omnetpp.ini


cd ~/git/Mobile6TiSCH-3-0-Chester/analysis/

# python3 main.py "/home/ubuntu/git/Mobile6TiSCH-3-0-Chester/simulations/results/sfcomparison_retron"
# python3 main.py "/home/ubuntu/git/Mobile6TiSCH-3-0-Chester/simulations/results/mobilityanalysis_pert"
python3 main.py "/home/ubuntu/git/Mobile6TiSCH-3-0-Chester/simulations/results/mobilityanalysis_scenario"
# python3 main.py "/home/ubuntu/git/Mobile6TiSCH-3-0-Chester/simulations/results/mobilityanalysis_batch"

cd ~/git/Mobile6TiSCH-3-0-Chester/simulations/