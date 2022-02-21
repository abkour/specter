# Ambient occlusion

The ambient occlusion algorithm considers surfaces around a point to compute the total amount of unobstructed
indirect light that the point receives. The distances to these occluding surfaces plays a factor, in that 
occluding surfaces far away have almost no effect on lighting, whereas surfaces in close proximity will 
occlude a lot of light.

The algorithm that is implemented as of this writing does not consider the distance to occluding surfaces.
Therefore it is not real AO.

## Difference between the correct and incorrect results

I could just remove the incorrect results and say that I have implemented AO correctly on my first try, but I thought
it would be interesting to have this as a reference for future self. When I implemented AO I got the general algorithm
right, however my results were incorrect. I was searching for a while to figure out what I was doing wrong. Until I figured
out that the mistake was in the way I was tracing shadow rays. Shadow rays necessarily originate from within the octree space.
Therefore, on intersection with a bbox the near distance is negative. My implementation was discarding intersections with negative
distances as an optimization. This generally works very well for rays outside the octree space, since the near distance is always
positive in that case.

It took me a while to figure out this issue. It mostly came down to thinking that the octree implementation was correct and 
not even considering it as a mistake. In the future, I need to discard my premises when they are unfound. I didn't have a 
reason to believe that the octree implementation was correct. Sure, it works for the point light solver, but that one does not
cast shadow rays.