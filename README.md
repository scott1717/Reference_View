<H1>Reference_View</H1> 

<p>Reference_View allows a collection of pointers (or smart pointers) to be iterated as a collection of references to the underlying type.</p>
<p>non const example:</p>
<pre><code class="language-pascal"><p>std::vector&ltint*&gt nums;</p>
<p>nums.push_back(new int(1));
nums.push_back(new int(2));</code></p>
<p>revive::Reference_View numsView(nums);</p>
<p>for(auto& i : numsView)
    std::cout &lt&lt ++i &lt&lt std::endl;</p></pre>
<p>or const:</p>
<pre><code class="language-pascal"><p>const revive::Reference_View cNumsView(nums);</p>
<p>for(auto& i : cNumsView)
    std::cout &lt&lt i &lt&lt std::endl;  // editing not allowed in const</p></code></pre>

<h3>Why:</h3>
<ul>
    <li><b>Because C++ does not allow collections of references.  </b><br>It is unavoidable that there will be times when you need to internally maintain a collection of pointers or smart pointers.  With Reference_View you can internally maintain a collection of pointers but still allow others to iterate that collection of pointers as a collection of references.</li>
    <li><b>Because protecting data matters.  </b><br>Protecting the data inside a collection of pointers is rather difficult.  Simply making the collection const will not do it.  Ironically, making the collection const will make the pointers const (so that their memory addresses are fixed), but will not stop one from changing the data pointed to by the pointers.  Think (T* const) as opposed to (T const* const).</li> 
    <li><b>Because other solutions that I found stink.  </b><br>Some have suggested maintaining a collection of Reference_Wrapper(s).  But the problem with Reference_Wrapper (until the dot operator can be overloaded) is that one must still use .get() in order to return a reference to the wrapped object.</li>
</ul>

UN FINISHED README... (more to come with examples)
