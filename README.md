<H1>Reference_View</H1> 

<p>Reference_View allows a collection of pointers to be iterated as a collection of references.</p>
<pre><code class="language-pascal"><p>std::vector&ltint*&gt nums;</p>
<p>nums.push_back(new int(1));
nums.push_back(new int(2));</code></p>
<p>revive::Reference_View numsView(nums);</p>
<p>for(auto& i : numsView)
    std::cout &lt&lt ++i &lt&lt std::endl;</p></pre>


<h3>Why:</h3>
<ul>
<li><p>Because C++ does not allow a collection of references</p> <p>One must either maintain a collection of objects or pointers.  Some have suggested maintaining a collection of std::reference_wrapper.  
</p><p><code> std::vector&ltstd::reference_wrapper&ltint&gt&gt;</code></p><p>The problem with Reference_Wrapper is that one must still use .get() in order to return a reference to the wrapped type.</p></li>
</ul>
<p>But that doesn't mean that we can not view a collection of pointers as a collection of references. </p>

UNDONE README...
