# Project "Bowling"
----
## Preview

### S.O.L.I.D. Principles

---

# Bowling rules

1) A bowling court has many **Lanes**
2) Each **Lane** can contains many **Players**
3) Each **Player** plays his own **Game**
4) Each game contains ***10*** **Frames**
5) Each **Player** plays one by another

***Score calculation:***

1) Every **Frame** has ***2*** **Trial**, except the 10th **Frame**, which can has ***3*** **Trials** if the first 2 contains 10 points each
2) The ***1st*** **Trial** can have 0-10 points
3) The ***2nd*** **Trial** can have 0-(10 - 1st_trial_points)
4) If the ***1st*** **Trial** contains 10 point, then the **Frame** marked as **Strike**
5) If the ***1st*** **Trial** contains <10 point, and the ***2nd*** contains the rest points, the **Frame** marked as **Spare**
6) If the sum of points after the ***1st*** and the ***2nd*** **Trials** contains <10 points, no additional marks are added to the **Frame**

---

# TODO:

- [ ] Implement each class according to the SOLID principles
- [ ] Cover each class with proper unit test
- [ ] Fill in readme

- [ ] Refactor CMakeLists.txt for unit tests