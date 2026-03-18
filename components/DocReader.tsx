'use client';

import ReactMarkdown from 'react-markdown';
import rehypeHighlight from 'rehype-highlight';
import 'highlight.js/styles/atom-one-dark.css';

export default function DocReader({ content }: { content: string }) {
  return (
    <article className="prose prose-invert prose-lg max-w-none 
      prose-headings:font-sans prose-headings:font-bold prose-headings:tracking-tight 
      prose-h1:text-5xl prose-h1:text-white prose-h2:text-3xl prose-h2:text-[#ccff00] prose-h2:border-b prose-h2:border-white/10 prose-h2:pb-2 
      prose-a:text-[#00D1FF] prose-a:no-underline hover:prose-a:underline 
      prose-pre:bg-[#0a0a0a] prose-pre:border prose-pre:border-white/10 prose-pre:rounded-xl 
      prose-code:text-[#ccff00] prose-code:font-mono prose-code:bg-white/5 prose-code:px-1.5 prose-code:py-0.5 prose-code:rounded">
      <ReactMarkdown rehypePlugins={[rehypeHighlight as any]}>
        {content}
      </ReactMarkdown>
    </article>
  );
}
