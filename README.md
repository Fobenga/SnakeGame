# SnakeGame
<p>Basic logic snake game made with a custom DirectX 2D Framework called Chili Framework<br />(<em><a href="https://github.com/planetchili/chili_framework">https://github.com/planetchili/chili_framework</a></em>)</p>
<h2>Controls</h2>
<p><strong>UP ARROW -</strong>&nbsp;Move the snake upwards.<strong><br />DOWN ARROW -&nbsp;</strong>Move the snake downwards.<strong><br />LEFT ARROW -&nbsp;</strong>Move the snake to the left.<strong><br />RIGHT ARROW</strong>&nbsp;<strong>-&nbsp;</strong>Move the snake to the right.<br /><strong>SHIFT -&nbsp;&nbsp;</strong>Speeds up the snake by 80%<br /><strong>CTRL + X -&nbsp;</strong>Closes the game while in the start menu.</p>
<h2>Logic</h2>
<blockquote>
<p style="text-align: left;"><strong>Food&nbsp;</strong>will appear randomly in the grid size of 1 around all the grid.</p>
<p style="text-align: left;"><strong>Level&nbsp;</strong>will increase after the Upper score fills completely.<br />Levels can or cannot spawn obstacles, as mentioned below.</p>
<p style="text-align: left;"><strong>Obstacles</strong>&nbsp;will spawn with a grid size of 1 in levels based in the formula:<br />(level &gt; 0 &amp;&amp; <em>level % 2 == 0)</em></p>
<p style="text-align: left;"><strong>Endgame</strong>&nbsp;will happens when the snake hits the wall, the head hits itself or the countdown of the level finishes.</p>
<p style="text-align: left;"><strong>Countdown</strong>&nbsp;is the amount of time that the player have to eat another food.<br />The countdown increases at every food eaten in the level, and decreases while the player is moving.<br />When the countdown fills up the entire grid, a new level starts.<br />When the countdown reaches 0, it is declared GAME OVER.</p>
</blockquote>
