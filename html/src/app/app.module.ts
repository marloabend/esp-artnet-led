import {NgModule} from '@angular/core';
import {BrowserModule} from '@angular/platform-browser';
import {AppComponent} from './app.component';
import {HttpClientModule} from '@angular/common/http';
import {ReactiveFormsModule} from '@angular/forms';
import {DeviceService} from './service/device.service';
import {DmxChannelComponent} from './component/dmx-channel/dmx-channel.component';
import {SecondsPipe} from './pipe/seconds.pipe';

@NgModule({
  imports: [
    BrowserModule,
    HttpClientModule,
    ReactiveFormsModule,
    SecondsPipe,
  ],
  declarations: [
    AppComponent,
    DmxChannelComponent,
  ],
  providers: [
    DeviceService,
  ],
  bootstrap: [AppComponent]
})
export class AppModule {}
