int sgn(double x){return x<-EPS?-1:x>EPS;}
struct pol {
	int n;vector<pt> p;
	pol(){}
	pol(vector<pt> _p){p=_p;n=p.size();}
	double area(){
		double r=0.;
		fore(i,0,n)r+=p[i]%p[(i+1)%n];
		return abs(r)/2; // negative if CW, positive if CCW
	}
	pt centroid(){ // (barycenter)
		pt r(0,0);double t=0;
		fore(i,0,n){
			r=r+(p[i]+p[(i+1)%n])*(p[i]%p[(i+1)%n]);
			t+=p[i]%p[(i+1)%n];
		}
		return r/t/3;
	}
	bool has(pt q){ // O(n)
		fore(i,0,n)if(ln(p[i],p[(i+1)%n]).seghas(q))return true;
		int cnt=0;
		fore(i,0,n){
			int j=(i+1)%n;
			int k=sgn((q-p[j])%(p[i]-p[j]));
			int u=sgn(p[i].y-q.y),v=sgn(p[j].y-q.y);
			if(k>0&&u<0&&v>=0)cnt++;
			if(k<0&&v<0&&u>=0)cnt--;
		}
		return cnt!=0;
	}
	void normalize(){ // (call before haslog, remove collinear first)
		if(p[2].left(p[0],p[1]))reverse(p.begin(),p.end());
		int pi=min_element(p.begin(),p.end())-p.begin();
		vector<pt> s(n);
		fore(i,0,n)s[i]=p[(pi+i)%n];
		p.swap(s);
	}
	bool haslog(pt q){ // O(log(n)) only CONVEX. Call normalize first
		if(q.left(p[0],p[1])||q.left(p.back(),p[0]))return false;
		int a=1,b=p.size()-1;  // returns true if point on boundary
		while(b-a>1){          // (change sign of EPS in left
			int c=(a+b)/2;       //  to return false in such case)
			if(!q.left(p[0],p[c]))a=c;
			else b=c;
		}
		return !q.left(p[a],p[a+1]);
	}
	pol cut(ln l){   // cut CONVEX polygon by line l
		vector<pt> q;  // returns part at left of l.pq
		fore(i,0,n){
			int d0=sgn(l.pq%(p[i]-l.p)),d1=sgn(l.pq%(p[(i+1)%n]-l.p));
			if(d0>=0)q.pb(p[i]);
			ln m(p[i],p[(i+1)%n]);
			if(d0*d1<0&&!(l/m))q.pb(l^m);
		}
		return pol(q);
	}
	double intercircle(circle c){ // area of intersection with circle
		double r=0.;
		fore(i,0,n){
			int j=(i+1)%n;double w=c.intertriangle(p[i],p[j]);
			if((p[j]-c.o)%(p[i]-c.o)>0)r+=w;
			else r-=w;
		}
		return abs(r);
	}
};
